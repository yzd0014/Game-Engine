#pragma once
#include <mutex>
#include <condition_variable>

#include "Core\DataType\SmartPtr.h"
#include "Core\Lua\lua.hpp"
#include "Core\Utility\Utility.h"
#include "EngineLow\EngineLow.h"
#include "Core\HashedString\HashedString.h"

#include "GameObject.h"
#include "GameObjectController.h"
#include "PhysicsInfo.h"
#include "Renderable.h"

namespace Engine {
	extern std::mutex m_mutex;
	extern std::condition_variable m_condVar;
	//void CreateActor(const char * i_pScriptFilename, SmartPtr<GameObject> &o_pActor, PhysicsInfo &o_phXInfo, Renderable &o_renderInfo);
	//GLib::Sprites::Sprite * CreateSprite(const char * i_pFilename);
}

class ActorCreationManager {
public:
	ActorCreationManager(int i_numOfActors);
	void CreateActor(const char * i_pScriptFilename, SmartPtr<GameObject> &o_pActor, PhysicsInfo &o_phXInfo, Renderable &o_renderInfo);
	void initACM();
	void shutDownACM();
private:
	int numOfActors;
	//bool isFinished;
	//lua_State * pLuaState;
	std::map<Engine::HashedString, Engine::JobSystem::JobQueueData *> Queues;
};