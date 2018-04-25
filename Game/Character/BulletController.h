#pragma once
#include "GameCommon\GameObject.h"
#include "GameCommon\GameObjectController.h"
#include "Core\DataType\WeakPtr.h"
#include "FakedGlobals\GlobalVarWrapper.h"

class BulletController : public GameObjectController {
public:
	BulletController(WeakPtr<GameObject> i_pGameObject, GlobalVarWrapper &i_globals);
	WeakPtr<GameObject> getGameObject() const override;
	void setGameObject(WeakPtr<GameObject> i_pObject) override;
	Vector2D updateGameObject() override;
	void hit(PhysicsInfo & i_phyXInfo) override;
	~BulletController() override;
private:
	WeakPtr<GameObject> m_pGameObject;
	GlobalVarWrapper &m_globals;
};