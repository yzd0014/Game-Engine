#pragma once
#include "Engine.h"
//#include "Core\DataType\WeakPtr.h"

class PlayerController : public GameObjectController {
public:
	PlayerController(){}
	PlayerController(WeakPtr<GameObject> i_pGameObject, Messaging * i_pMessages);
	void setGameObject(WeakPtr<GameObject> i_pObject);
	WeakPtr<GameObject> getGameObject() const;
	Vector2D updateGameObject() override;
	void HandleButtonMovement(unsigned int i_VKey, bool b_WentDown);
	~PlayerController() override;

	Vector2D forceApplied;
	//PlayerController(const PlayerController & i_other);//copy constructor
	//PlayerController & operator = (const PlayerController & i_i_other);//assignment operator
	//PlayerController(PlayerController && i_other);//move copy constructor
	//PlayerController & operator = (PlayerController && i_i_other);//move assignment operator
private:
	WeakPtr<GameObject> m_pGameObject;
	Messaging * m_pMessages;
	//bool isforceApplied;
	//char userInput;
};