#include "SmartMonsterController.h"
#include "Engine.h"

SmartMonsterController::SmartMonsterController(WeakPtr<GameObject> i_pGameObject, const WeakPtr<GameObject> i_pFocusObject):
gameObject(i_pGameObject),
focusObject(i_pFocusObject)
{
}


void SmartMonsterController::setGameObject(WeakPtr<GameObject> i_pObject) {
	gameObject = i_pObject;
}

WeakPtr<GameObject> SmartMonsterController::getGameObject() const {
	return gameObject;
}

void SmartMonsterController::updateGameObject() {
	//assert(gameObject);

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