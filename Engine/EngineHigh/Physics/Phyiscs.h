#pragma once
#include <vector>
#include "Core\Math\Vector2D.h"
#include "GameCommon\PhysicsInfo.h"
#include "EngineHigh\CollisionSystem\CollisionPair.h"
namespace Physics {
	void updateFrameVelocity(vector<PhysicsInfo> &o_physicsInfos);
	void updateVelocity(PhysicsInfo & o_physicsInfo, float i_dt);
	void moveObjectsForward(vector<PhysicsInfo> &o_physicsInfos, float timeSpan);
	void resolveCollision(collisionPair &o_earliestCollision);
	void updateAllObjects(vector<PhysicsInfo> & o_physicsInfos, float i_dt);
}