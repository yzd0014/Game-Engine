#include <iostream>
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <time.h>

#include "Character/SmartMonsterController.h"
#include "Character/PlayerController.h"
#include "Engine.h"
bool Collision_UnitTest() {
	Engine::initHeap(10 * 1024 * 1024, 2048);

	{
		//event begin play
		SmartPtr<GameObject> gameObject_A(new GameObject("A", Vector2D(0, 0), 100));
		gameObject_A->zRotationDegree = -90;
		AABB A_BoudningBox;
		A_BoudningBox.center = Vector2D(0, 0);
		A_BoudningBox.extends = Vector2D(1, 2);
		PhysicsInfo A_info(gameObject_A, 0, 0);
		A_info.velocity = Vector2D(10, 0);
		A_info.frameVelocity = A_info.velocity;
		A_info.m_BoundingBox = A_BoudningBox;

		SmartPtr<GameObject> gameObject_B(new GameObject("B", Vector2D(5, 0), 100));
		gameObject_B->zRotationDegree = 0;
		AABB B_BoudningBox;
		B_BoudningBox.center = Vector2D(0, 0);
		B_BoudningBox.extends = Vector2D(1, 1);
		PhysicsInfo B_info(gameObject_B, 0, 0);
		B_info.velocity = Vector2D(0, 0);
		B_info.frameVelocity = B_info.velocity;
		B_info.m_BoundingBox = B_BoudningBox;

		//simulation of game loop
		for (int i = 0; i < 20; i++) {
			Vector4D normal_A;
			Vector4D normal_B;
			float ColTime;
			Engine::CollisionDetection(A_info, B_info, 1, normal_A, ColTime);
			normal_B = -1 * normal_A;
			gameObject_A->position = gameObject_A->position + A_info.velocity;
			gameObject_B->position = gameObject_B->position + B_info.velocity;
		}

	}
	
	Engine::shutDownHeap();
	return true;
}