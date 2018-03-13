#pragma once
#include "Core\Math\Vector2D.h"
#include "GameObject.h"

inline Vector2D GameObject::getPosition() const {
	return position;
}

inline void GameObject::setPosition(const Vector2D &i_position) {
	position = i_position;
}

inline void GameObject::setName(const char *i_pName) {
	pName = _strdup(i_pName ? i_pName : "Unnamed");
}
inline void GameObject::setID(unsigned int i_ID) {
	ID = i_ID;
}

inline uint8_t GameObject::getLife() {
	return life;
}
inline void  GameObject::setLife(uint8_t i_life) {
	life = i_life;
}

inline unsigned int GameObject::getID() const{
	return ID;
}

inline char * GameObject::getName() const {
	return pName;
}