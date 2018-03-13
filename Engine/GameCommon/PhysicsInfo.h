#pragma once
#include "GameObject.h"
#include "Core\DataType\WeakPtr.h"
#include "EngineHigh\CollisionSystem\AABB.h"
class PhysicsInfo {
public:
	PhysicsInfo(): 
		m_pObject(nullptr),
		velocity(Vector2D(0, 0)),
		mass(0),
		drag(0)
	{

	}
	PhysicsInfo(WeakPtr<GameObject> i_pObject, float i_mass, float i_drag) :
	m_pObject(i_pObject),
	mass(i_mass),
	drag(i_drag),
	velocity(Vector2D(0, 0))
	{

	}
	inline WeakPtr<GameObject> getGameObject() {
		return m_pObject;
	}
	inline void setGameObject(WeakPtr<GameObject> i_pObject) {
		m_pObject = i_pObject;
	}
	inline Vector2D getVelocity() {
		return velocity;
	}
	inline void setVelocity(Vector2D i_v) {
		velocity = i_v;
	}
	inline float getMass() {
		return mass;
	}
	inline float getDrag() {
		return drag;
	}
//private:
	WeakPtr<GameObject> m_pObject;
	Vector2D velocity;
	AABB m_BoundingBox;
	float mass;
	float drag;
};