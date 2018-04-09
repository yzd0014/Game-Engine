#pragma once
//#include "GameObject.h"
#include "Core\DataType\WeakPtr.h"
#include "Core\Math\Vector2D.h"
class GameObjectController {
public:
	virtual ~GameObjectController() {}
	//virtual void setGameObject(WeakPtr<GameObject> i_pObject) = 0;
	//virtual WeakPtr<GameObject> getGameObject() const = 0;
	virtual Vector2D updateGameObject() = 0;
};