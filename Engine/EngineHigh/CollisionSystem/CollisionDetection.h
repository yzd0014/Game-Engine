#pragma once
#include "GameCommon\PhysicsInfo.h"
namespace Engine {
	void CollisionDetection(PhysicsInfo & i_object_A, PhysicsInfo & i_object_B, Vector2D velocity_A, Vector2D velocity_B, float i_dt);
}
