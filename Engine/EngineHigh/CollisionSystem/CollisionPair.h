#pragma once
#include "GameCommon\PhysicsInfo.h"
struct collisionPair {
	float collisionTime;
	Vector4D collisionNormal4A;
	PhysicsInfo * collisionObjects[2];
};