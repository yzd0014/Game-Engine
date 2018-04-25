#include "AsteroidController.h"
#include "Engine.h"

AsteroidController::AsteroidController(WeakPtr<GameObject> i_pGameObject):
	m_pGameObject(i_pGameObject)
{
	hitEventOverided = false;
	physicsVel = Vector2D(0.0f, 0.0f);
}

Vector2D AsteroidController::updateGameObject() {
	SmartPtr<GameObject> temp_gameObject = m_pGameObject.Aquire();
	if (temp_gameObject->position.x > 640) temp_gameObject->position.x = -640;
	else if (temp_gameObject->position.x < -640)temp_gameObject->position.x = 640;
	if (temp_gameObject->position.y > 360) temp_gameObject->position.y = -360;
	else if (temp_gameObject->position.y < -360) temp_gameObject->position.y = 360;

	return Vector2D(0.0f, 0.0f);
}

void AsteroidController::hit(PhysicsInfo & i_phyXInfo) {

}
WeakPtr<GameObject> AsteroidController::getGameObject() const {
	return m_pGameObject;
}
void AsteroidController::setGameObject(WeakPtr<GameObject> i_pObject) {
	m_pGameObject = i_pObject;
}

AsteroidController::~AsteroidController() {

}