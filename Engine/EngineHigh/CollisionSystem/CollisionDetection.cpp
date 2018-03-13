#include "GameCommon\PhysicsInfo.h"
#include "Core\Core.h"
#include "assert.h"
#include <algorithm> 
namespace Engine {
	void CollisionDetection(PhysicsInfo & i_object_A, PhysicsInfo & i_object_B, Vector2D velocity_A, Vector2D velocity_B, float i_dt) {
		//get A to world matrix
		SmartPtr<GameObject> pGameObject_A = i_object_A.getGameObject().Aquire();
		Matrix4x4 A2World = Matrix4x4::CreateZRotation(pGameObject_A->zRotationDegree);
		Vector2D position_A = pGameObject_A->position;
		A2World = Matrix4x4::CreateTranslation(position_A.x, position_A.y, 0) * A2World;

		//get B to world matrix
		SmartPtr<GameObject> pGameObject_B = i_object_B.getGameObject().Aquire();
		Matrix4x4 B2World = Matrix4x4::CreateZRotation(pGameObject_B->zRotationDegree);
		Vector2D position_B = pGameObject_B->position;
		B2World = Matrix4x4::CreateTranslation(position_B.x, position_B.y, 0) * B2World;

/////////////////////////////////////////////////////////////////////////////////////////////////		
		//get B to A matrix 
			//caculate time on x aix
		Matrix4x4 World2A = A2World.getInverse();
		Matrix4x4 B2A = World2A * B2World;
		Vector2D BRelativeToA = velocity_B - velocity_A;
		Vector4D BVelocityInA(BRelativeToA.x, BRelativeToA.y, 0, 0);
		BVelocityInA = World2A * BVelocityInA;
		
		Vector4D v_TopRight(i_object_B.m_BoundingBox.extends.x, i_object_B.m_BoundingBox.extends.y, 0, 0);
		v_TopRight = B2A * v_TopRight;
		float rightProjectionOnX = abs(v_TopRight.x);
		Vector4D v_TopLeft(-i_object_B.m_BoundingBox.extends.x, i_object_B.m_BoundingBox.extends.y, 0, 0);
		v_TopLeft = B2A * v_TopLeft;
		float leftProjectionOnX = abs(v_TopLeft.x);
		float BProjectionOnX;
		if (leftProjectionOnX > rightProjectionOnX)BProjectionOnX = leftProjectionOnX;
		else BProjectionOnX = rightProjectionOnX;
		Vector4D BCenter(i_object_B.m_BoundingBox.center.x, i_object_B.m_BoundingBox.center.y, 0, 1);
		Vector4D BCenterInA = B2A * BCenter;


		float A_extends = i_object_A.m_BoundingBox.extends.x + BProjectionOnX;
		float A_left = i_object_A.m_BoundingBox.center.x - A_extends;
		float A_right = i_object_A.m_BoundingBox.center.x + A_extends;
		float D_close = A_left - BCenterInA.x;
		float D_open = A_right - BCenterInA.x;

		float T_close_B2Ax;
		float T_open_B2Ax;
		if (BVelocityInA.x != 0) {
			T_close_B2Ax = D_close / BVelocityInA.x;
			T_open_B2Ax = D_open / BVelocityInA.x;
		}
		if (BVelocityInA.x == 0) {
			if (BCenterInA.x > A_left && BCenterInA.x < A_right) {
				T_close_B2Ax = 0;
				T_open_B2Ax = i_dt;
			}
			else return;
		}
		if (T_close_B2Ax * T_open_B2Ax < 0) {
			T_close_B2Ax = 0;
			T_open_B2Ax = i_dt;
		}
		if (T_close_B2Ax <= 0 && T_open_B2Ax <= 0) {
			return;
		}
		if (T_close_B2Ax > T_open_B2Ax) {
			float temp = T_close_B2Ax;
			T_close_B2Ax = T_open_B2Ax;
			T_open_B2Ax = temp;
		}
		if (T_close_B2Ax > i_dt) {
			return;
		}	
			//caculate time on y aix
		float rightProjectionOnY = abs(v_TopRight.y);
		float leftProjectionOnY = abs(v_TopLeft.y);
		float BProjectionOnY;
		if (leftProjectionOnY > rightProjectionOnY) BProjectionOnY = leftProjectionOnY;
		else BProjectionOnY = rightProjectionOnY;

		A_extends = i_object_A.m_BoundingBox.extends.y + BProjectionOnY;
		float A_bottom = i_object_A.m_BoundingBox.center.y - A_extends;
		float A_top = i_object_A.m_BoundingBox.center.y + A_extends;
		D_close = A_bottom - BCenterInA.y;
		D_open = A_top - BCenterInA.y;

		float T_close_B2Ay;
		float T_open_B2Ay;
		if (BVelocityInA.y != 0) {
			T_close_B2Ay = D_close / BVelocityInA.y;
			T_open_B2Ay = D_open / BVelocityInA.y;
		}
		if (BVelocityInA.y == 0) {
			if (BCenterInA.y > A_bottom && BCenterInA.y < A_top) {
				T_close_B2Ay = 0;
				T_open_B2Ay = i_dt;
			}
			else {
				return;
			}
		}

		if (T_close_B2Ay * T_open_B2Ay < 0) {
			T_close_B2Ay = 0;
			T_open_B2Ay = i_dt;
		}
		if (T_close_B2Ay <= 0 && T_open_B2Ay <= 0) {
			return;
		}
		if (T_close_B2Ay > T_open_B2Ay) {
			float temp = T_close_B2Ay;
			T_close_B2Ay = T_open_B2Ay;
			T_open_B2Ay = temp;
		}
		if (T_close_B2Ay > i_dt) {
			return;
		}

////////////////////////////////////////////////////////////////////////////////////////////////////////
		//get A to B Matrix
			//caculate time on x aix
		Matrix4x4 World2B = B2World.getInverse();
		Matrix4x4 A2B = World2B * A2World;
		Vector2D ARelativeToB = velocity_A - velocity_B;
		Vector4D AVelocityInB(ARelativeToB.x, ARelativeToB.y, 0, 0);
		AVelocityInB = World2B * AVelocityInB;

		v_TopRight = Vector4D(i_object_A.m_BoundingBox.extends.x, i_object_A.m_BoundingBox.extends.y, 0, 0);
		v_TopRight = A2B * v_TopRight;
		rightProjectionOnX = abs(v_TopRight.x);
		v_TopLeft = Vector4D(-i_object_A.m_BoundingBox.extends.x, i_object_A.m_BoundingBox.extends.y, 0, 0);
		v_TopLeft = A2B * v_TopLeft;
		leftProjectionOnX = abs(v_TopLeft.x);
		float AProjectionOnX;
		if (leftProjectionOnX > rightProjectionOnX) AProjectionOnX = leftProjectionOnX;
		else AProjectionOnX = rightProjectionOnX;
		Vector4D ACenter(i_object_A.m_BoundingBox.center.x, i_object_A.m_BoundingBox.center.y, 0, 1);
		Vector4D ACenterInB = A2B * ACenter;

		float B_extends = i_object_B.m_BoundingBox.extends.x + AProjectionOnX;
		float B_left = i_object_B.m_BoundingBox.center.x - B_extends;
		float B_right = i_object_B.m_BoundingBox.center.x + B_extends;
		D_close = B_left - ACenterInB.x;
		D_open = B_right - ACenterInB.x;

		float T_close_A2Bx;
		float T_open_A2Bx;
		if (AVelocityInB.x != 0) {
			T_close_A2Bx = D_close / AVelocityInB.x;
			T_open_A2Bx = D_open / AVelocityInB.x;
		}
		if (AVelocityInB.x == 0) {
			if (ACenterInB.x > B_left && ACenterInB.x < B_right) {
				T_close_A2Bx = 0;
				T_open_A2Bx = i_dt;
			}
			else return;
		}

		if (T_close_A2Bx * T_open_A2Bx < 0) {
			T_close_A2Bx = 0;
			T_open_A2Bx = i_dt;
		}
		if (T_close_A2Bx <= 0 && T_open_A2Bx <= 0)return;
		if (T_close_A2Bx > T_open_A2Bx) {
			float temp = T_close_A2Bx;
			T_close_A2Bx = T_open_A2Bx;
			T_open_A2Bx = temp;
		}
		if (T_close_A2Bx > i_dt) return;

			//cacluate time on y aix
		rightProjectionOnY = abs(v_TopRight.y);
		leftProjectionOnY = abs(v_TopLeft.y);
		float AProjectionOnY;
		if (leftProjectionOnY > rightProjectionOnY) AProjectionOnY = leftProjectionOnY;
		else AProjectionOnY = rightProjectionOnY;

		B_extends = i_object_B.m_BoundingBox.extends.y + AProjectionOnY;
		float B_bottom = i_object_B.m_BoundingBox.center.y - B_extends;
		float B_top = i_object_B.m_BoundingBox.center.y + B_extends;
		D_close = B_bottom - ACenterInB.y;
		D_open = B_top - ACenterInB.y;

		float T_close_A2By;
		float T_open_A2By;
		if (AVelocityInB.y != 0) {
			T_close_A2By = D_close / AVelocityInB.y;
			T_open_A2By = D_open / AVelocityInB.y;
		}
		if (AVelocityInB.y  == 0) {
			if (ACenterInB.y > B_bottom && ACenterInB.y < B_top) {
				T_close_A2By = 0;
				T_open_A2By = i_dt;
			}
			else {
				return;
			}
		}
		if (T_close_A2By * T_open_A2By < 0) {
			T_close_A2By = 0;
			T_open_A2By = i_dt;
		}
		if (T_close_A2By <= 0 && T_open_A2By <= 0) {
			return;
		}
		if (T_close_A2By > T_open_A2By) {
			float temp = T_close_A2By;
			T_close_A2By = T_open_A2By;
			T_open_A2By = temp;
		}
		if (T_close_A2By > i_dt)return;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
		float allCloseTime[] = {T_close_B2Ax, T_close_B2Ay, T_close_A2Bx, T_close_A2By};
		float allOpenTime[] = { T_open_B2Ax, T_open_B2Ay, T_open_A2Bx, T_open_A2By };

		float maxClose = *std::max_element(allCloseTime, allCloseTime + 4);
		float minOpen = *std::min_element(allOpenTime, allOpenTime + 4);

		if (maxClose < minOpen) {
			DEBUG_PRINT_INFO("There is collision.");
		}
	}
}