#include "PlayerController.h"

PlayerController::PlayerController(WeakPtr<GameObject> i_pGameObject):
gameObject (i_pGameObject)
{	
}

void PlayerController::setGameObject(WeakPtr<GameObject> i_pObject){
	gameObject = i_pObject;
}
WeakPtr<GameObject> PlayerController::getGameObject() const {
	return gameObject;
}

void PlayerController::updateGameObject() {
	
}

void PlayerController::getDirectionFromUser(char i_userInput) {
	//Vector2D output;
	userInput = i_userInput;
}

PlayerController::~PlayerController() {
	//delete gameObject;
}



/*PlayerController::PlayerController(const PlayerController & i_other) {//copy constructor
GameObject * mGameObject = new GameObject();
*mGameObject = *i_other.gameObject;
gameObject = mGameObject;
}
PlayerController & PlayerController::operator=(const PlayerController & i_other) {//assignment operator
if (gameObject) {
delete gameObject;
}
GameObject * mGameObject = new GameObject();
*mGameObject = *i_other.gameObject;
gameObject = mGameObject;
return *this;
}
PlayerController::PlayerController(PlayerController && i_other):
gameObject(i_other.gameObject)
{//move copy constructor
i_other.gameObject = nullptr;
}
PlayerController & PlayerController::operator=(PlayerController && i_other) {//move assignment operator
std::swap(gameObject, i_other.gameObject);
return *this;
}*/