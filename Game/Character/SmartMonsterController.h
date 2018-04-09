#pragma once
#include "Engine.h"
#include "Core\DataType\WeakPtr.h"
class SmartMonsterController : public GameObjectController {
public:
	SmartMonsterController(WeakPtr<GameObject> i_pGameObject, WeakPtr<GameObject> i_pFocusObject);
	void setGameObject(WeakPtr<GameObject> i_pObject);
	WeakPtr<GameObject> getGameObject() const;
	Vector2D updateGameObject() override;
	void setFocusObject(WeakPtr<GameObject> i_pObject);
	~SmartMonsterController() override;

	//SmartMonsterController(const SmartMonsterController &i_other);
	//SmartMonsterController & operator=(const SmartMonsterController &i_other);
	//SmartMonsterController(SmartMonsterController &&i_other);
	//SmartMonsterController & operator=(SmartMonsterController &&i_other);
private:
	WeakPtr<GameObject> gameObject;
	WeakPtr<GameObject> focusObject;
};