#pragma once
#include "Core\Math\Vector2D.h"
#include "GameCommon\PhysicsInfo.h"
namespace Physics {
	Vector2D update(PhysicsInfo & i_physicsInfo, const Vector2D & i_Forces, float i_dt);
}