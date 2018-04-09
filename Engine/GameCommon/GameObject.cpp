#include "GameObject.h"
#include "Core\Utility\Utility.h"
#include "Core\Math\Vector2D.h"

unsigned int GameObject::IDPool = 0;
GameObject::GameObject() {}
GameObject::GameObject(const char * i_pName, const Vector2D &i_position, uint8_t i_life):
//velocity(Vector2D(0, 0)),
//mass(1000),
position(i_position),
zRotationDegree(0),
ID(IDPool),
life(i_life),
pName(dupstring(i_pName ? i_pName : "Unnamed")),
m_pController(nullptr)
{//constructor
	IDPool++;
}

GameObject::GameObject(const GameObject &i_other):
	//velocity(i_other.velocity),
	//mass(i_other.mass),
position (i_other.position),
ID(i_other.ID),
life(i_other.life),
pName(dupstring(i_other.pName ? i_other.pName : "Unnamed")),
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
	life = i_other.life;
	m_pController = nullptr;
	//mass = i_other.mass;
	//velocity = i_other.velocity;
	return *this;
}

GameObject::GameObject(GameObject &&i_other):
	//velocity(i_other.velocity),
	//mass(i_other.mass),
position(i_other.position),
ID(i_other.ID),
life(i_other.life),
pName(i_other.pName)
{//move copy constructor
	i_other.pName = nullptr;
	m_pController = nullptr;
}
GameObject & GameObject::operator=(GameObject &&i_other) {//move assignment operator
	std::swap(ID, i_other.ID);
	std::swap(pName, i_other.pName);
	std::swap(position, i_other.position);
	std::swap(life, i_other.life);
	m_pController = nullptr;
	//std::swap(mass, i_other.mass);
	//std::swap(velocity, i_other.velocity);
	return *this;
}
GameObject::~GameObject() {
	if (pName) {
		delete[] pName;
	}
}

