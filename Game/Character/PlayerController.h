#pragma once
#include "Engine.h"
#include "Core\DataType\WeakPtr.h"

class PlayerController : public GameObjectController {
public:
	PlayerController(WeakPtr<GameObject> i_pGameObject);
	void setGameObject(WeakPtr<GameObject> i_pObject) override;
	WeakPtr<GameObject> getGameObject() const override;
	void updateGameObject() override;
	void getDirectionFromUser(char i_userInput);
	~PlayerController() override;

	//PlayerController(const PlayerController & i_other);//copy constructor
	//PlayerController & operator = (const PlayerController & i_i_other);//assignment operator
	//PlayerController(PlayerController && i_other);//move copy constructor
	//PlayerController & operator = (PlayerController && i_i_other);//move assignment operator
private:
	WeakPtr<GameObject> gameObject;
	Vector2D dir;//player's move displacement
	char userInput;
};