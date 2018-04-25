#pragma once
#include "Core\Math\Vector2D.h"
#include "Core\DataType\SmartPtr.h"
#include "GameObjectController.h"
//class GameObjectController;

class GameObject {
public:
	GameObject();
	GameObject(const char * i_pName, const Vector2D &i_position, uint8_t i_life);//constructor
	GameObject(const GameObject &i_ther);//copy constructor
	GameObject & operator=(const GameObject &i_other);// assignment operator
	GameObject(GameObject &&i_other);//move copy constructor
	GameObject & operator=(GameObject &&i_other);//move assignment operator
	inline Vector2D getPosition() const;
	inline void setPosition(const Vector2D &i_position);
	inline void setName(const char *i_pName);
	inline char * getName() const;
	inline unsigned int getID() const;
	inline void setID(unsigned int i_ID);
	inline uint8_t getLife();
	inline void setLife(uint8_t i_life);
	~GameObject();
	
	//Vector2D forwardVector;
	Vector2D position;
	float zRotationDegree;
	unsigned int ID;
	static unsigned int IDPool;
	uint8_t life;
	char * pName;
	bool active;
	SmartPtr<GameObjectController> m_pController;
};

#include "GameObject-inl.h"