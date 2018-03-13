#pragma once
#include "GameObject.h"
#include "Core\DataType\WeakPtr.h"
class GameObjectController {
public:
	virtual void setGameObject(WeakPtr<GameObject> i_pObject) = 0;
	virtual WeakPtr<GameObject> getGameObject() const = 0;
	virtual void updateGameObject() = 0;
	virtual ~GameObjectController() = 0;
};