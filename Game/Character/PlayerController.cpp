#include "PlayerController.h"
#include "Core\DataType\Delegate.h"
#include "assert.h"

PlayerController::PlayerController(WeakPtr<GameObject> i_pGameObject, Messaging * i_pMessages) :
	forceApplied(Vector2D(0.0f, 0.0f)),
	m_pGameObject(i_pGameObject),
	m_pMessages(i_pMessages)
{
	assert(m_pMessages);
	m_pMessages->ButtonChangeReceivers.AddDelegate(Delegate<unsigned int, bool>::Create<PlayerController, &PlayerController::HandleButtonMovement>(this));
}

void PlayerController::setGameObject(WeakPtr<GameObject> i_pObject){
	m_pGameObject = i_pObject;
}
WeakPtr<GameObject> PlayerController::getGameObject() const {
	return m_pGameObject;
}

Vector2D PlayerController::updateGameObject() {
	return Vector2D(0.0f, 0.0f);
}
void PlayerController::HandleButtonMovement(unsigned int i_VKey, bool b_WentDown) {
	if (i_VKey == 0x57 && forceApplied == Vector2D(0.0f, 0.0f) && b_WentDown) {
		forceApplied = Vector2D(0.0f, 1000.0f);
		return;

	}
	if (i_VKey == 0x53 && forceApplied == Vector2D(0.0f, 0.0f) && b_WentDown) {
		forceApplied = Vector2D(0.0f, -1000.0f);
		return;

	}
	if (i_VKey == 0x41 && forceApplied == Vector2D(0.0f, 0.0f) && b_WentDown) {
		forceApplied = Vector2D(-1000.0f, 0.0f);
		return;

	}
	if (i_VKey == 0x44 && forceApplied == Vector2D(0.0f, 0.0f) && b_WentDown) {
		forceApplied = Vector2D(1000.0f, 0.0f);
		return;
	}
	//release force
	if (i_VKey == 0x57 && forceApplied == Vector2D(0.0f, 1000.0f) && !b_WentDown) {
		forceApplied = Vector2D(0.0f, 0.0f);
		return;
	}
	if (i_VKey == 0x53 && forceApplied == Vector2D(0.0f, -1000.0f) && !b_WentDown) {
		forceApplied = Vector2D(0.0f, 0.0f);
		return;
	}
	if (i_VKey == 0x41 && forceApplied == Vector2D(-1000.0f, 0.0f) && !b_WentDown) {
		forceApplied = Vector2D(0.0f, 0.0f);
		return;
	}
	if (i_VKey == 0x44 && forceApplied == Vector2D(1000.0f, 0.0f) && !b_WentDown) {
		forceApplied = Vector2D(0.0f, 0.0f);
		return;
	}
}
PlayerController::~PlayerController() {
	assert(m_pMessages);
	m_pMessages->ButtonChangeReceivers.RemoveDelegate(Delegate<unsigned int, bool>::Create<PlayerController, &PlayerController::HandleButtonMovement>(this));
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
}

void PlayerController::getDirectionFromUser(char i_userInput) {
//Vector2D output;
userInput = i_userInput;
}

*/