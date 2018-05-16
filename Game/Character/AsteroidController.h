#pragma once
#include "GameCommon\GameObjectController.h"
#include "GameCommon\GameObject.h"
#include "Core\DataType\WeakPtr.h"
class AsteroidController : public GameObjectController {
public:
	AsteroidController(WeakPtr<GameObject> i_pGameObject);
	WeakPtr<GameObject> getGameObject() const override;
	void setGameObject(WeakPtr<GameObject> i_pObject) override;
	void updateGameObject() override;
	void hit(PhysicsInfo & i_phyXInfo) override;
	~AsteroidController() override;
private:
	WeakPtr<GameObject> m_pGameObject;
};
