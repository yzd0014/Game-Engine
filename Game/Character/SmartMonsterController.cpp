#include "SmartMonsterController.h"
#include "Engine.h"

SmartMonsterController::SmartMonsterController(WeakPtr<GameObject> i_pGameObject, const WeakPtr<GameObject> i_pFocusObject):
gameObject(i_pGameObject),
focusObject(i_pFocusObject)
{
	hitEventOverided = true;
	physicsVel = Vector2D(0.0f, 0.0f);
}


void SmartMonsterController::setGameObject(WeakPtr<GameObject> i_pObject) {
	gameObject = i_pObject;
}

WeakPtr<GameObject> SmartMonsterController::getGameObject() const {
	return gameObject;
}

Vector2D SmartMonsterController::updateGameObject() {
	//assert(gameObject);
	Vector2D force4Monster;
	SmartPtr<GameObject> pPlayer = focusObject.Aquire();
	SmartPtr<GameObject> m_pMonster = gameObject.Aquire();
	Vector2D dir = pPlayer->position - m_pMonster->position;

	if (dir.get_x() != 0 || dir.get_y() != 0) {
		dir.normalize();
		//dir = dir * 0.01f;
		force4Monster = dir * 200.0f;
	}
	else {
		force4Monster = Vector2D(0.0f, 0.0f);
	}

	return force4Monster;
}

void SmartMonsterController::hit(PhysicsInfo & i_phyXInfo) {
	SmartPtr<GameObject> objectHit = i_phyXInfo.m_pObject.Aquire();
	char * nameExpected = {"Ship"};
	if (strcmp(objectHit->pName, nameExpected) == 0) {
		DEBUG_PRINT_INFO("Ship is hit by Asteroid");
	}
}

void SmartMonsterController::setFocusObject(WeakPtr<GameObject> i_pObject) {
	focusObject = i_pObject;
}

SmartMonsterController::~SmartMonsterController() {
	//delete gameObject;
}


/*
SmartMonsterController::SmartMonsterController(const SmartMonsterController &i_other) {
GameObject * mGameObject = new GameObject();
*mGameObject = *i_other.gameObject;
gameObject = mGameObject;

focusObject = i_other.focusObject;
}
SmartMonsterController &SmartMonsterController:: operator=(const SmartMonsterController &i_other) {
if (gameObject) {
delete gameObject;
}
GameObject * mGameObject = new GameObject();
*mGameObject = *i_other.gameObject;
gameObject = mGameObject;

focusObject = i_other.focusObject;
return *this;
}
SmartMonsterController::SmartMonsterController(SmartMonsterController &&i_other):
gameObject (i_other.gameObject),
focusObject (i_other.focusObject)
{
i_other.gameObject = nullptr;
}
SmartMonsterController &SmartMonsterController:: operator=(SmartMonsterController &&i_other) {
std::swap(gameObject, i_other.gameObject);
std::swap(focusObject, i_other.focusObject);
return *this;
}

*/