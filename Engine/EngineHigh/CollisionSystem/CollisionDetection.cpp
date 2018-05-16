#include "Core\Profiling\Profiling.h"
#include "GameCommon\PhysicsInfo.h"
#include "Core\Core.h"
#include "CollisionPair.h"
#include "assert.h"
#include <algorithm>
#include <vector>
#define fakeZero -0.000001f
namespace Engine {
	bool CollisionDetection(PhysicsInfo & i_object_A, PhysicsInfo & i_object_B, float i_dt, Vector4D &o_normal4A, float &o_collisionTime) {
		PROFILE_UNSCOPED(collisionDetection_foo);
		Vector2D velocity_A = i_object_A.frameVelocity;
		Vector2D velocity_B = i_object_B.frameVelocity;

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
		float D_close_Ax = A_left - BCenterInA.x;
		float D_open_Ax = A_right - BCenterInA.x;

		float T_close_B2Ax;
		float T_open_B2Ax;
		
		if (floatEqual(D_close_Ax, 0.0f))D_close_Ax = 0.0f;
		if (floatEqual(D_open_Ax, 0.0f))D_open_Ax = 0.0f;

		float AMinusB_x = i_object_A.m_BoundingBox.center.x - BCenterInA.x;//cache the relative locoation
		if (BVelocityInA.x != 0) {
			T_close_B2Ax = D_close_Ax / BVelocityInA.x;
			T_open_B2Ax = D_open_Ax / BVelocityInA.x;
			//there are three possible situations in terms of object B's state when calculation is done in A's coordinate system
			//1. B is approaching A
			//2. B is in between A
			//3. B is moving away from A
			if (T_close_B2Ax <= 0 && T_open_B2Ax <= 0) {// B is moving away from A, so there is no collision
				return false;
			}
			if (T_close_B2Ax * T_open_B2Ax < 0) {// B is in between A, collision happens at the very begining of delta time, set overlap flag
				if (BVelocityInA.x > 0) {
					T_close_B2Ax = fakeZero;
				}
				else {
					T_open_B2Ax = T_close_B2Ax;
					T_close_B2Ax = fakeZero;
				}
			}

			if (T_close_B2Ax > T_open_B2Ax) {//B is approaching A, swap close and open time to hanle negative velocity
				float temp = T_close_B2Ax;
				T_close_B2Ax = T_open_B2Ax;
				T_open_B2Ax = temp;
			}
			if (T_close_B2Ax > i_dt) {//B is apporaching A, collision won't happen within current delta time
				return false;
			}
		}
		else {
			if (BCenterInA.x > A_left && BCenterInA.x < A_right) {
				T_close_B2Ax = fakeZero;
				T_open_B2Ax = i_dt;
			}
			else return false;
		}
		//choose max close time
		int index_MaxElement = 0;
		float maxClose = T_close_B2Ax;
		//caculate time on y aix
		float rightProjectionOnY = abs(v_TopRight.y);
		float leftProjectionOnY = abs(v_TopLeft.y);
		float BProjectionOnY;
		if (leftProjectionOnY > rightProjectionOnY) BProjectionOnY = leftProjectionOnY;
		else BProjectionOnY = rightProjectionOnY;

		A_extends = i_object_A.m_BoundingBox.extends.y + BProjectionOnY;
		float A_bottom = i_object_A.m_BoundingBox.center.y - A_extends;
		float A_top = i_object_A.m_BoundingBox.center.y + A_extends;
		float D_close_Ay = A_bottom - BCenterInA.y;
		float D_open_Ay = A_top - BCenterInA.y;

		float T_close_B2Ay;
		float T_open_B2Ay;

		if (floatEqual(D_close_Ay, 0.0f))D_close_Ay = 0.0f;
		if (floatEqual(D_open_Ay, 0.0f))D_open_Ay = 0.0f;

		float AMinusB_y = i_object_A.m_BoundingBox.center.y - BCenterInA.y;//cache the relative locoation
		if (BVelocityInA.y != 0) {
			
			T_close_B2Ay = D_close_Ay / BVelocityInA.y;
			T_open_B2Ay = D_open_Ay / BVelocityInA.y;

			if (T_close_B2Ay <=0  && T_open_B2Ay <= 0) {
				return false;
			}
			if (T_close_B2Ay * T_open_B2Ay < 0) {
				if (BVelocityInA.y > 0) {
					T_close_B2Ay = fakeZero;
				}
				else {
					T_open_B2Ay = T_close_B2Ay;
					T_close_B2Ay = fakeZero;
				}
			}

			if (T_close_B2Ay > T_open_B2Ay) {
				float temp = T_close_B2Ay;
				T_close_B2Ay = T_open_B2Ay;
				T_open_B2Ay = temp;
			}
			if (T_close_B2Ay > i_dt) {
				return false;
			}
		}
		else {
			if (BCenterInA.y > A_bottom && BCenterInA.y < A_top) {
				T_close_B2Ay = fakeZero;
				T_open_B2Ay = i_dt;
			}
			else {
				return false;
			}
		}
		//choose max close time
		if (T_close_B2Ay > maxClose) {
			maxClose = T_close_B2Ay;
			index_MaxElement = 1;
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
		float D_close_Bx = B_left - ACenterInB.x;
		float D_open_Bx = B_right - ACenterInB.x;

		float T_close_A2Bx;
		float T_open_A2Bx;

		if (floatEqual(D_close_Bx, 0.0f))D_close_Bx = 0.0f;
		if (floatEqual(D_open_Bx, 0.0f))D_open_Bx = 0.0f;

		float BMinusA_x = i_object_B.m_BoundingBox.center.x - ACenterInB.x;//cache the relative locoation
		if (AVelocityInB.x != 0) {
			
			T_close_A2Bx = D_close_Bx / AVelocityInB.x;
			T_open_A2Bx = D_open_Bx / AVelocityInB.x;

			if (T_close_A2Bx <= 0  && T_open_A2Bx <= 0)return false;
			if (T_close_A2Bx * T_open_A2Bx < 0) {
				if (AVelocityInB.x > 0) {
					T_close_A2Bx = fakeZero;
				}
				else {
					T_open_A2Bx = T_close_A2Bx;
					T_close_A2Bx = fakeZero;
				}
			}
			if (T_close_A2Bx > T_open_A2Bx) {
				float temp = T_close_A2Bx;
				T_close_A2Bx = T_open_A2Bx;
				T_open_A2Bx = temp;
			}
			if (T_close_A2Bx > i_dt) return false;
		}
		else {
			if (ACenterInB.x > B_left && ACenterInB.x < B_right) {
				T_close_A2Bx = fakeZero;
				T_open_A2Bx = i_dt;
			}
			else return false;
		}
		//choose max close time
		if (T_close_A2Bx > maxClose) {
			maxClose = T_close_A2Bx;
			index_MaxElement = 2;
		}
		//cacluate time on y aix
		rightProjectionOnY = abs(v_TopRight.y);
		leftProjectionOnY = abs(v_TopLeft.y);
		float AProjectionOnY;
		if (leftProjectionOnY > rightProjectionOnY) AProjectionOnY = leftProjectionOnY;
		else AProjectionOnY = rightProjectionOnY;

		B_extends = i_object_B.m_BoundingBox.extends.y + AProjectionOnY;
		float B_bottom = i_object_B.m_BoundingBox.center.y - B_extends;
		float B_top = i_object_B.m_BoundingBox.center.y + B_extends;
		float D_close_By = B_bottom - ACenterInB.y;
		float D_open_By = B_top - ACenterInB.y;

		float T_close_A2By;
		float T_open_A2By;

		if (floatEqual(D_close_By, 0.0f))D_close_By = 0.0f;
		if (floatEqual(D_open_By, 0.0f))D_open_By = 0.0f;

		float BMinusA_y = i_object_B.m_BoundingBox.center.y - ACenterInB.y;//cache the relative locoation
		if (AVelocityInB.y != 0) {
			
			T_close_A2By = D_close_By / AVelocityInB.y;
			T_open_A2By = D_open_By / AVelocityInB.y;

			if (T_close_A2By <= 0  && T_open_A2By <= 0) {
				return false;
			}
			if (T_close_A2By * T_open_A2By < 0) {
				if (AVelocityInB.y > 0) {
					T_close_A2By = fakeZero;
				}
				else {
					T_open_A2By = T_close_A2By;
					T_close_A2By = fakeZero;
				}
			}
			if (T_close_A2By > T_open_A2By) {
				float temp = T_close_A2By;
				T_close_A2By = T_open_A2By;
				T_open_A2By = temp;
			}
			if (T_close_A2By > i_dt)return false;
		}
		else {
			if (ACenterInB.y > B_bottom && ACenterInB.y < B_top) {
				T_close_A2By = fakeZero;
				T_open_A2By = i_dt;
			}
			else {
				return false;
			}
		}
		//choose max close time
		if (T_close_A2By > maxClose) {
			maxClose = T_close_A2By;
			index_MaxElement = 3;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		float allOpenTime[] = { T_open_B2Ax, T_open_B2Ay, T_open_A2Bx, T_open_A2By };
		float minOpen = *std::min_element(allOpenTime, allOpenTime + 4);

		if (maxClose <= minOpen) {
			DEBUG_PRINT_INFO("There is collision.");
			//cacluate normals based on seperation plane
			if (index_MaxElement == 0) {
				if (AMinusB_x > 0) o_normal4A = Vector4D(1, 0, 0, 0);
				else o_normal4A = Vector4D(-1, 0, 0, 0);
				o_normal4A = A2World * o_normal4A;
			}
			else if (index_MaxElement == 1) {
				if (AMinusB_y > 0) o_normal4A = Vector4D(0, 1, 0, 0);
				else o_normal4A = Vector4D(0, -1, 0, 0);
				o_normal4A = A2World * o_normal4A;
			}
			else if (index_MaxElement == 2) {
				if (BMinusA_x > 0) o_normal4A = Vector4D(-1, 0, 0, 0);
				else o_normal4A = Vector4D(1, 0, 0, 0);
				o_normal4A = B2World * o_normal4A;
			}
			else if (index_MaxElement == 3) {
				if (BMinusA_y > 0) o_normal4A = Vector4D(0, -1, 0, 0);
				else o_normal4A = Vector4D(0, 1, 0, 0);
				o_normal4A = B2World * o_normal4A;
			}
			o_collisionTime = maxClose;
			return true;
		}
		return false;
	}

	bool findEarliestCollision(vector<PhysicsInfo> &i_allPhysXInfos, float i_dt, collisionPair &o_earliestCollision) {
		o_earliestCollision.collisionTime = i_dt;
		bool foundCollision_earliest = false;
		size_t count = i_allPhysXInfos.size();
		
		for (size_t i = 0; i < count - 1; i++) {
			for (size_t j = i + 1; j < count; j++) {
				Vector4D colNormal4A;
				float colTime;
				bool collisionFound = CollisionDetection(i_allPhysXInfos[i], i_allPhysXInfos[j], i_dt, colNormal4A, colTime);
				if (collisionFound == true && colTime == fakeZero) {
					//broadcast hit event
					SmartPtr<GameObject> temp_gameObject_A = i_allPhysXInfos[i].m_pObject.Aquire();
					if (temp_gameObject_A->m_pController->hitEventOverided == true) {
						temp_gameObject_A->m_pController->hit(i_allPhysXInfos[j]);
					}

					SmartPtr<GameObject> temp_gameObject_B = i_allPhysXInfos[j].m_pObject.Aquire();
					if (temp_gameObject_B->m_pController->hitEventOverided == true) {
						temp_gameObject_B->m_pController->hit(i_allPhysXInfos[i]);
					}
				}
				if (collisionFound == true && colTime != fakeZero) {
					if (colTime < o_earliestCollision.collisionTime) {
						o_earliestCollision.collisionTime = colTime;
						o_earliestCollision.collisionNormal4A = colNormal4A;
						o_earliestCollision.collisionObjects[0] = &i_allPhysXInfos[i];
						o_earliestCollision.collisionObjects[1] = &i_allPhysXInfos[j];
					}
					foundCollision_earliest = true;
				}
			}
		}
		//broadcast hit event
		if (foundCollision_earliest == true) {
			SmartPtr<GameObject> temp_gameObject_A = o_earliestCollision.collisionObjects[0]->m_pObject.Aquire();
			if (temp_gameObject_A->m_pController->hitEventOverided == true) {
				temp_gameObject_A->m_pController->hit(*o_earliestCollision.collisionObjects[1]);
			}
			SmartPtr<GameObject> temp_gameObject_B = o_earliestCollision.collisionObjects[1]->m_pObject.Aquire();
			if (temp_gameObject_B->m_pController->hitEventOverided == true) {
				temp_gameObject_B->m_pController->hit(*o_earliestCollision.collisionObjects[0]);
			}
		}
		return foundCollision_earliest;
	}

}