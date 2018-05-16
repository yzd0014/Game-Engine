#include "BulletController.h"
#include "AsteroidController.h"
#include "Engine.h"
#include <stdlib.h> 

#define PI 3.14159265
BulletController::BulletController(WeakPtr<GameObject> i_pGameObject, GlobalVarWrapper &i_globals):
m_pGameObject(i_pGameObject),
m_globals(i_globals)
{
	hitEventOverided = true;
}

void BulletController::updateGameObject() {
	//delete bullet when it's out of range
	SmartPtr<GameObject> temp_gameObject = m_pGameObject.Aquire();
	if (temp_gameObject->position.x > 640) {
		temp_gameObject->active = false;
	}	
	else if (temp_gameObject->position.x < -640) {
		temp_gameObject->active = false;
	}
	if (temp_gameObject->position.y > 360) {
		temp_gameObject->active = false;
	}
	else if (temp_gameObject->position.y < -360) {
		temp_gameObject->active = false;
	}
}

void BulletController::hit(PhysicsInfo & i_phyXInfo) {
	SmartPtr<GameObject> pSelfObject = m_pGameObject.Aquire();
	SmartPtr<GameObject> pObjectHit = i_phyXInfo.m_pObject.Aquire();
	char * nameExpected = { "RockBig" };
	if (strcmp(pObjectHit->pName, nameExpected) == 0 && pSelfObject->active == true && pObjectHit->active == true) {
		pSelfObject->active = false;//delete the bullet itself
		pObjectHit->active = false;//delete the hit big asteroid

		//create two small asteroids
		for (int i = 0; i < 2; i++) {
			//gameObject creation
			SmartPtr<GameObject> pRockSmall = new GameObject("RockSmall", pObjectHit->position, 100);
			pRockSmall->zRotationDegree = 45;
			m_globals.m_AllActors.push_back(pRockSmall);

			//phyiscsInfo creation
			m_globals.m_AllPhysXInfos.push_back(PhysicsInfo(pRockSmall, 1.0f, 0.0001f));
			m_globals.m_AllPhysXInfos.back().m_BoundingBox = m_globals.m_rockSmallBox;
			float randomAngle = (float)(rand() % 360);
			float vel_x = (float)(200 * cos(randomAngle / 180 * PI));
			float vel_y = (float)(200 * sin(randomAngle / 180 * PI));
			m_globals.m_AllPhysXInfos.back().setVelocity(Vector2D(vel_x, vel_y));

			//rnederable creation
			Renderable rockSmallRenderable;
			rockSmallRenderable.m_pObject = pRockSmall;
			rockSmallRenderable.m_pSprite = m_globals.m_rockSmallSprite;
			m_globals.m_AllRenderables.push_back(rockSmallRenderable);

			//controller creation
			m_globals.m_asteroidControllers.push_back(new AsteroidController(pRockSmall));
			m_globals.m_asteroidControllers.back()->gameObjectIndex = (int)m_globals.m_AllActors.size() - 1;
			m_globals.m_asteroidControllers.back()->phyXInfoIndex = (int)m_globals.m_AllPhysXInfos.size() - 1;
			m_globals.m_asteroidControllers.back()->renderableIndex = (int)m_globals.m_AllRenderables.size() - 1;
			pRockSmall->m_pController = m_globals.m_asteroidControllers.back();
		}
	}
	nameExpected = { "RockSmall" };
	if (strcmp(pObjectHit->pName, nameExpected) == 0 && pSelfObject->active == true && pObjectHit->active == true) {
		pSelfObject->active = false;//delete the bullet itself
		pObjectHit->active = false;//delete the hit big asteroid
	}
}
WeakPtr<GameObject> BulletController::getGameObject() const {
	return m_pGameObject;
}

void BulletController::setGameObject(WeakPtr<GameObject> i_pObject) {
	m_pGameObject = i_pObject;
}

BulletController::~BulletController() {

}