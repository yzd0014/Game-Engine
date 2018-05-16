#pragma once
#include "Core\DataType\WeakPtr.h"
#include "Core\Math\Vector2D.h"
class PhysicsInfo;
class GameObject;
class GameObjectController {
public:
	virtual ~GameObjectController() {}
	virtual void updateGameObject() = 0;
	virtual void hit(PhysicsInfo & i_phyXInfo) = 0;//function to respond for collision
	virtual WeakPtr<GameObject> getGameObject() const = 0;
	virtual void setGameObject(WeakPtr<GameObject> i_pObject) = 0;
	
	bool hitEventOverided;
	int gameObjectIndex;
	int phyXInfoIndex;
	int renderableIndex;
};