#pragma once
#include "GameCommon\PhysicsInfo.h"
#include "Core\Math\Vector4D.h"
#include "CollisionPair.h"
namespace Engine {
	bool CollisionDetection(PhysicsInfo & i_object_A, PhysicsInfo & i_object_B, float i_dt, Vector4D &o_normal4A, float &o_collisionTime);
	bool findEarliestCollision(vector<PhysicsInfo> &i_allPhysXInfosfloat, float i_dt, collisionPair &o_earliestCollision);
}
