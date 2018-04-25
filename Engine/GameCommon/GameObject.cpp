#include "GameObject.h"
#include "Core\Utility\Utility.h"
#include "Core\Math\Vector2D.h"

unsigned int GameObject::IDPool = 0;
GameObject::GameObject() {}
GameObject::GameObject(const char * i_pName, const Vector2D &i_position, uint8_t i_life):
//forwardVector(Vector2D(0, 1)),
position(i_position),
zRotationDegree(0),
ID(IDPool),
life(i_life),
pName(dupstring(i_pName ? i_pName : "Unnamed")),
active(true),
m_pController(nullptr)
{//constructor
	IDPool++;
}

GameObject::GameObject(const GameObject &i_other):
//forwardVector(i_other.forwardVector),
position (i_other.position),
zRotationDegree(i_other.zRotationDegree),
ID(i_other.ID),
life(i_other.life),
pName(dupstring(i_other.pName ? i_other.pName : "Unnamed")),
active(i_other.active),
m_pController(nullptr)
{//copy constructor
	IDPool++;
}

GameObject & GameObject::operator=(const GameObject &i_other) {//assignment operator
	if (pName) {
		delete[] pName;
	}
	ID = i_other.ID;
	pName = dupstring(i_other.pName ? i_other.pName : "Unnamed");
	position = i_other.position;
	zRotationDegree = i_other.zRotationDegree;
	life = i_other.life;
	active = i_other.active;
	m_pController = nullptr;
	//forwardVector = i_other.forwardVector;
	return *this;
}

GameObject::GameObject(GameObject &&i_other):
//forwardVector(i_other.forwardVector),
position(i_other.position),
zRotationDegree(i_other.zRotationDegree),
ID(i_other.ID),
life(i_other.life),
pName(i_other.pName),
active(i_other.active)
{//move copy constructor
	m_pController = nullptr;
}
GameObject & GameObject::operator=(GameObject &&i_other) {//move assignment operator
	std::swap(ID, i_other.ID);
	std::swap(pName, i_other.pName);
	std::swap(position, i_other.position);
	std::swap(zRotationDegree, i_other.zRotationDegree);
	std::swap(life, i_other.life);
	std::swap(active, i_other.active);
	m_pController = nullptr;
	return *this;
}
GameObject::~GameObject() {
	if (pName) {
		delete[] pName;
	}
}

