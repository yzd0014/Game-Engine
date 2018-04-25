#include "PlayerController.h"
#include "Engine.h"
#include "assert.h"
#include "BulletController.h"
#include <math.h> 
#define PI 3.14159265
PlayerController::PlayerController(WeakPtr<GameObject> i_pGameObject, Messaging * i_pMessages, GlobalVarWrapper &i_globals) :
	m_pGameObject(i_pGameObject),
	m_pMessages(i_pMessages),
	m_globals(i_globals),
	upKeyHold(false),
	leftKeyHold(false),
	rightKeyHold(false)
{
	assert(m_pMessages);
	m_pMessages->ButtonChangeReceivers.AddDelegate(Delegate<unsigned int, bool>::Create<PlayerController, &PlayerController::HandleButtonMovement>(this));
	hitEventOverided = true;
	physicsVel = Vector2D(0.0f, 0.0f);
}

void PlayerController::setGameObject(WeakPtr<GameObject> i_pObject){
	m_pGameObject = i_pObject;
}
WeakPtr<GameObject> PlayerController::getGameObject() const {
	return m_pGameObject;
}

Vector2D PlayerController::updateGameObject() {
	SmartPtr<GameObject> temp_gameObject = m_pGameObject.Aquire();
	Vector2D drivenForece(0.0f, 0.0f);
	if (temp_gameObject->active == true) {
		float absRot = temp_gameObject->zRotationDegree + 90;//absolute rotation of the player
		if (upKeyHold == true) {//apply forward force
			if (physicsVel.getMagnitude() < 600) {
				float force_x = (float)(800.0f * cos(absRot / 180 * PI));
				float force_y = (float)(800.0f * sin(absRot / 180 * PI));
				drivenForece = Vector2D(force_x, force_y);
			}
		}

		float rotRate_degree = 360;
		if (leftKeyHold == true) {//add delta rotation to left
			temp_gameObject->zRotationDegree = temp_gameObject->zRotationDegree + rotRate_degree * Engine::getDeltaTime();
		}
		else if (rightKeyHold == true) {//add delta rotation to right
			temp_gameObject->zRotationDegree = temp_gameObject->zRotationDegree - rotRate_degree * Engine::getDeltaTime();
		}
		///////////////////////////////////////////////////////////////////////////////
		if (temp_gameObject->position.x > 640) temp_gameObject->position.x = -640;
		else if (temp_gameObject->position.x < -640)temp_gameObject->position.x = 640;
		if (temp_gameObject->position.y > 360) temp_gameObject->position.y = -360;
		else if (temp_gameObject->position.y < -360) temp_gameObject->position.y = 360;

	}
	return drivenForece;
}

void PlayerController::hit(PhysicsInfo & i_phyXInfo) {
	SmartPtr<GameObject> pSelfObj = m_pGameObject.Aquire();
	pSelfObj->active = false;
	pSelfObj->position = Vector2D(0, 500);
	m_globals.m_AllPhysXInfos[0].setVelocity(Vector2D(0, 0));
}

void PlayerController::HandleButtonMovement(unsigned int i_VKey, bool b_WentDown) {
	SmartPtr<GameObject> pSelfObj = m_pGameObject.Aquire();
	if (pSelfObj->active == true) {
		//up key
		if (i_VKey == 0x57 && b_WentDown) {
			upKeyHold = true;
			return;
		}
		if (i_VKey == 0x57 && !b_WentDown) {
			upKeyHold = false;
			return;
		}
		//left
		if (i_VKey == 0x41 && b_WentDown) {
			leftKeyHold = true;
			return;
		}
		if (i_VKey == 0x41 && !b_WentDown) {
			leftKeyHold = false;
			return;
		}
		//right
		if (i_VKey == 0x44 && b_WentDown) {
			rightKeyHold = true;
			return;
		}
		if (i_VKey == 0x44 && !b_WentDown) {
			rightKeyHold = false;
			return;
		}
		//space bar
		if (i_VKey == 0x20 && b_WentDown) {
			LONGLONG tickDiff = Timing::getCurrentTick() - tickStamp;
			float timeDiff = Timing::getTimeDiff_ms(tickDiff);
			if (timeDiff >= 70) {
				tickStamp = Timing::getCurrentTick();

				SmartPtr<GameObject> temp_gameObject = m_pGameObject.Aquire();
				float absRot = temp_gameObject->zRotationDegree + 90;//absolute rotation of the player in terms positive X
																	 //get bullet's location
				float bulletOffset = 30.0f;
				float bulletOffset_x = (float)(bulletOffset * cos(absRot / 180 * PI));
				float bulletOffset_y = (float)(bulletOffset * sin(absRot / 180 * PI));
				Vector2D bulletPos(temp_gameObject->position.x + bulletOffset_x, temp_gameObject->position.y + bulletOffset_y);

				//create new gameObject for bullet
				SmartPtr<GameObject> pBullet = new GameObject("Bullet", bulletPos, 100);
				m_globals.m_AllActors.push_back(pBullet);
				pBullet->zRotationDegree = temp_gameObject->zRotationDegree;

				//create new phyiscsInfo for bullet
				m_globals.m_AllPhysXInfos.push_back(PhysicsInfo(pBullet, 1.0f, 0.0f));
				m_globals.m_AllPhysXInfos.back().m_BoundingBox = m_globals.m_bulletBox;
				float bulletVelValue = 1000.0f;
				float vel_x = (float)(bulletVelValue * cos(absRot / 180 * PI));
				float vel_y = (float)(bulletVelValue * sin(absRot / 180 * PI));
				m_globals.m_AllPhysXInfos.back().setVelocity(Vector2D(vel_x, vel_y));

				//create new renderable for bullet
				Renderable bulletRenderable;
				bulletRenderable.m_pObject = pBullet;
				bulletRenderable.m_pSprite = m_globals.m_bulletSprite;
				m_globals.m_AllRenderables.push_back(bulletRenderable);

				//create new controller for bullet
				m_globals.m_bulletControllers.push_back(new BulletController(pBullet, m_globals));
				m_globals.m_bulletControllers.back()->gameObjectIndex = (int)m_globals.m_AllActors.size() - 1;
				m_globals.m_bulletControllers.back()->phyXInfoIndex = (int)m_globals.m_AllPhysXInfos.size() - 1;
				m_globals.m_bulletControllers.back()->renderableIndex = (int)m_globals.m_AllRenderables.size() - 1;
				pBullet->m_pController = m_globals.m_bulletControllers.back();
			}
			return;
		}
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