#pragma once
#include<iostream>
using namespace std;
class Vector2D {
public:
	Vector2D(float i_x, float i_y);
	Vector2D();
	//setter
	inline void set_x(float i_x);
	inline void set_y(float i_y);
	//getter
	inline float get_x() const;
	inline float get_y() const;
	
	inline void normalize();
	inline float getMagnitude();
//private:
	float x, y;
};

inline Vector2D operator+(const Vector2D& l_value, const Vector2D& r_value);
inline Vector2D operator-(const Vector2D& l_value, const Vector2D& r_value);
inline ostream & operator<<(ostream &out, const Vector2D &obj);
inline istream & operator >> (istream &in, Vector2D &obj);
inline Vector2D operator*(const Vector2D& l_value, float r_value);
inline Vector2D operator*(float l_value, const Vector2D& r_value);
inline float dot(const Vector2D& l_value, const Vector2D& r_value);
inline bool operator==(const Vector2D& l_value, const Vector2D& r_value);

#include "Vector2D-inl.h"