#include "ActorCreationManager.h"

#include <mutex>
#include <condition_variable>

#include "Core\DataType\SmartPtr.h"
#include "Core\Lua\lua.hpp"
#include "Core\Utility\Utility.h"
#include "EngineLow\EngineLow.h"


#include "GameObject.h"
#include "GameObjectController.h"
#include "PhysicsInfo.h"
#include "Renderable.h"


GLib::Sprites::Sprite * CreateSprite(const char * i_pFilename)
{
	Engine::m_mutex.lock();
	assert(i_pFilename);

	size_t sizeTextureFile = 0;

	// Load the source file (texture data)
	void * pTextureFile = Engine::LoadFile(i_pFilename, sizeTextureFile);

	// Ask GLib to create a texture out of the data (assuming it was loaded successfully)
	GLib::Texture * pTexture = pTextureFile ? GLib::CreateTexture(pTextureFile, sizeTextureFile) : nullptr;

	// exit if something didn't work
	// probably need some debug logging in here!!!!
	if (pTextureFile)
		delete[] pTextureFile;

	if (pTexture == nullptr)
		return NULL;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int depth = 0;

	// Get the dimensions of the texture. We'll use this to determine how big it is on screen
	bool result = GLib::GetDimensions(pTexture, width, height, depth);
	assert(result == true);
	assert((width > 0) && (height > 0));

	// Define the sprite edges
	GLib::Sprites::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
	GLib::Sprites::SpriteUVs	UVs = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f } };
	GLib::RGBA							Color = { 255, 255, 255, 255 };

	// Create the sprite
	GLib::Sprites::Sprite * pSprite = GLib::Sprites::CreateSprite(Edges, 0.1f, Color, UVs);
	if (pSprite == nullptr)
	{
		GLib::Release(pTexture);
		return nullptr;
	}

	// Bind the texture to sprite
	GLib::Sprites::SetTexture(*pSprite, *pTexture);
	
	Engine::m_mutex.unlock();
	
	return pSprite;
}

void ReadFloatArrary(lua_State *i_pState, int i_index, float * o_Array, size_t i_numFloats) {
	lua_pushnil(i_pState);

	size_t index = 0;
	while (lua_next(i_pState, i_index - 1) != 0) {
		assert(lua_type(i_pState, -1) == LUA_TNUMBER);
		o_Array[index] = (float)lua_tonumber(i_pState, -1);
		index++;
		lua_pop(i_pState, 1);
		if (index >= i_numFloats) {
			lua_pop(i_pState, 1);
			break;
		}
	}
}

namespace Engine {
	std::mutex m_mutex;
	std::condition_variable m_condVar;
}


class ProcessFile : public Engine::JobSystem::IJob
{
public:
	ProcessFile(const char * i_pFilename, int &i_numOfActors, SmartPtr<GameObject> &o_pActor, PhysicsInfo &o_phXInfo, Renderable &o_renderInfo, std::map<Engine::HashedString, Engine::JobSystem::JobQueueData *> &Queues) :
		IJob("ProcessFile"),
		m_pFilename(i_pFilename),
		//m_luaState(i_luaState),
		numOfActors(i_numOfActors),
		pActor(o_pActor),
		phXInfo(o_phXInfo),
		renderInfo(o_renderInfo),
		Queues(Queues)
	{
		assert(m_pFilename);
	}
	void Run()
	{

		if (m_pFilename)
		{
			lua_State * pLuaState = luaL_newstate();
			assert(pLuaState);

			luaL_openlibs(pLuaState);

			size_t sizeFileContents = 0;
			void * pFileContents = Engine::LoadFile(m_pFilename, sizeFileContents);

			if (pFileContents && sizeFileContents)
			{
				int result = 0;
				result = luaL_loadbuffer(pLuaState, reinterpret_cast<char *>(pFileContents), sizeFileContents, nullptr);
				assert(result == 0);
				result = lua_pcall(pLuaState, 0, 0, 0);
				assert(result == 0);

				result = lua_getglobal(pLuaState, "Actor");
				assert(result == LUA_TTABLE);

				//name 
				const char * pName = nullptr;
				lua_pushstring(pLuaState, "name");
				result = lua_gettable(pLuaState, -2);
				if (result == LUA_TSTRING) {
					pName = lua_tostring(pLuaState, -1);
					assert(pName != nullptr);
					pName = dupstring(pName);
				}
				else {
					DEBUG_PRINT_INFO("Didn't find string looking for name. \n");
					pName = dupstring("None");
				}
				lua_pop(pLuaState, 1);

				//position
				float position[2];
				lua_pushstring(pLuaState, "position");
				result = lua_gettable(pLuaState, -2);
				assert(result == LUA_TTABLE);
				ReadFloatArrary(pLuaState, -1, position, 2);
				lua_pop(pLuaState, 1);

				//health
				uint8_t health;
				lua_pushstring(pLuaState, "health");
				result = lua_gettable(pLuaState, -2);
				if (result == LUA_TNUMBER) {
					health = (uint8_t)lua_tonumber(pLuaState, -1);
				}
				else {
					DEBUG_PRINT_INFO("Didn't find string looking for health. \n");
					health = 0;
				}
				lua_pop(pLuaState, 1);

				//mass
				float mass;
				lua_pushstring(pLuaState, "mass");
				result = lua_gettable(pLuaState, -2);
				if (result == LUA_TNUMBER) {
					mass = (float)lua_tonumber(pLuaState, -1);
				}
				else {
					DEBUG_PRINT_INFO("Didn't find string looking for mass. \n");
					mass = 100;
				}
				lua_pop(pLuaState, 1);

				//drag
				float drag;
				lua_pushstring(pLuaState, "drag");
				result = lua_gettable(pLuaState, -2);
				if (result == LUA_TNUMBER) {
					drag = (float)lua_tonumber(pLuaState, -1);
				}
				else {
					DEBUG_PRINT_INFO("Didn't find string looking for drag. \n");
					drag = 100;
				}
				lua_pop(pLuaState, 1);

				//sprite
				const char * pSpriteDir = nullptr;
				lua_pushstring(pLuaState, "sprite");
				result = lua_gettable(pLuaState, -2);
				assert(result == LUA_TSTRING);
				pSpriteDir = lua_tostring(pLuaState, -1);
				assert(pSpriteDir != nullptr);
				pSpriteDir = dupstring(pSpriteDir);
				lua_pop(pLuaState, 1);

				lua_pop(pLuaState, 1);
				int stack_items = lua_gettop(pLuaState);
				assert(stack_items == 0);

				pActor = new GameObject(pName, Vector2D(position[0], position[1]), health);
				delete[] pName;
				phXInfo = PhysicsInfo(pActor, mass, drag);
				renderInfo.m_pObject = pActor;
				renderInfo.m_pSprite = CreateSprite(pSpriteDir);
				delete[] pSpriteDir;
			}

			delete[] pFileContents;
			lua_close(pLuaState);

			std::unique_lock<std::mutex> lck(Engine::m_mutex);
			numOfActors = numOfActors - 1;
			if (numOfActors == 0) {//notify all actors have been created
				Engine::m_condVar.notify_all();
			}
		}
	}

private:
	const char *		m_pFilename;
	//lua_State * m_luaState;
	int &numOfActors;
	SmartPtr<GameObject> &pActor;
	PhysicsInfo &phXInfo;
	Renderable &renderInfo;
	std::map<Engine::HashedString, Engine::JobSystem::JobQueueData *> &Queues;
};

ActorCreationManager::ActorCreationManager(int i_numOfActors):
numOfActors(i_numOfActors)
//isFinished(false)
//pLuaState(luaL_newstate())
{

}

void ActorCreationManager::CreateActor(const char * i_pScriptFilename, SmartPtr<GameObject> &o_pActor, PhysicsInfo &o_phXInfo, Renderable &o_renderInfo) {
	
	ProcessFile * pProcessActor = new ProcessFile(i_pScriptFilename, numOfActors, o_pActor, o_phXInfo, o_renderInfo, Queues);
	assert(pProcessActor);
	Engine::JobSystem::RunJob(pProcessActor, "Default", Queues);
}

void ActorCreationManager::initACM() {
	Engine::JobSystem::CreateQueue("Default", 4, Queues);
}

void ActorCreationManager::shutDownACM() {
	std::unique_lock<std::mutex> lck(Engine::m_mutex);
	while (numOfActors != 0) {
		Engine::m_condVar.wait(lck);
	}
	
	Engine::JobSystem::Shutdown(Queues);

}