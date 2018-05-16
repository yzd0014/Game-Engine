#pragma once
#include "GameCommon\GameObject.h"
#include "GameCommon\GameObjectController.h"
#include "Core\DataType\WeakPtr.h"
#include "Core\Messaging\Messaging.h"
#include "FakedGlobals\GlobalVarWrapper.h"

class PlayerController : public GameObjectController {
public:
	PlayerController(WeakPtr<GameObject> i_pGameObject, Messaging * i_pMessages, GlobalVarWrapper &i_globals);
	void setGameObject(WeakPtr<GameObject> i_pObject) override;
	WeakPtr<GameObject> getGameObject() const override;
	void updateGameObject() override;
	void HandleButtonMovement(unsigned int i_VKey, bool b_WentDown);
	void hit(PhysicsInfo & i_phyXInfo) override;
	~PlayerController() override;

private:
	WeakPtr<GameObject> m_pGameObject;
	Messaging * m_pMessages;
	GlobalVarWrapper &m_globals;
	bool upKeyHold;
	bool leftKeyHold;
	bool rightKeyHold;
	LONGLONG tickStamp;//used for fire rate
	LONGLONG spawnTick;//used for unbeaten phase
};