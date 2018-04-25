#include "ActorCreationManager.h"

#include <mutex>
#include <condition_variable>

#include "Core\DataType\SmartPtr.h"
#include "Core\Lua\lua.hpp"
#include "Core\Utility\Utility.h"
#include "EngineLow\EngineLow.h"
#include "EngineHigh\CollisionSystem\AABB.h"

#include "GameObject.h"
#include "GameObjectController.h"
#include "PhysicsInfo.h"
#include "Renderable.h"

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
	ProcessFile(const char * i_pFilename, int &i_numOfActors, SmartPtr<GameObject> &o_pActor, PhysicsInfo &o_phXInfo, 
		AABB i_actorBB, Renderable & o_renderable, GLib::Sprites::Sprite * i_Sprite, std::map<Engine::HashedString, Engine::JobSystem::JobQueueData *> &Queues) :
		IJob("ProcessFile"),
		m_pFilename(i_pFilename),
		//m_luaState(i_luaState),
		numOfActors(i_numOfActors),
		m_pActor(o_pActor),
		m_phXInfo(o_phXInfo),
		m_actorBB(i_actorBB),
		m_renderable(o_renderable),
		m_Sprite(i_Sprite),
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

				//initial rotation
				float zRotation;
				lua_pushstring(pLuaState, "zRotation");
				result = lua_gettable(pLuaState, -2);
				if (result == LUA_TNUMBER) {
					zRotation = (float)lua_tonumber(pLuaState, -1);
				}
				else {
					DEBUG_PRINT_INFO("Didn't find string looking for health. \n");
					zRotation = 0.0f;
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

				//initial velocity
				float velocity[2];
				lua_pushstring(pLuaState, "velocity");
				result = lua_gettable(pLuaState, -2);
				assert(result == LUA_TTABLE);
				ReadFloatArrary(pLuaState, -1, velocity, 2);
				lua_pop(pLuaState, 1);

				lua_pop(pLuaState, 1);
				int stack_items = lua_gettop(pLuaState);
				assert(stack_items == 0);
//******************************************************************************************************
				//initialize new gameObject
				m_pActor = new GameObject(pName, Vector2D(position[0], position[1]), 100);
				delete[] pName;
				m_pActor->zRotationDegree = zRotation;

				//initialize new phyiscsInfo
				m_phXInfo = PhysicsInfo(m_pActor, mass, drag);
				m_phXInfo.m_BoundingBox = m_actorBB;
				m_phXInfo.setVelocity(Vector2D(velocity[0], velocity[1]));

				//initialize new renderable
				m_renderable.m_pObject = m_pActor;
				m_renderable.m_pSprite = m_Sprite;
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
	int &numOfActors;
	SmartPtr<GameObject> & m_pActor;
	PhysicsInfo & m_phXInfo;
	AABB m_actorBB;
	Renderable & m_renderable;
	GLib::Sprites::Sprite * m_Sprite;
	std::map<Engine::HashedString, Engine::JobSystem::JobQueueData *> &Queues;
};

ActorCreationManager::ActorCreationManager(int i_numOfActors):
numOfActors(i_numOfActors)
//isFinished(false)
//pLuaState(luaL_newstate())
{

}

void ActorCreationManager::CreateActor(const char * i_pScriptFilename, SmartPtr<GameObject> &o_pActor, 
	PhysicsInfo &o_phXInfo, AABB i_actorBB, Renderable & o_renderable, GLib::Sprites::Sprite * i_Sprite) {

	ProcessFile * pProcessActor = new ProcessFile(i_pScriptFilename, numOfActors, o_pActor, o_phXInfo, 
		i_actorBB, o_renderable, i_Sprite, Queues);
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