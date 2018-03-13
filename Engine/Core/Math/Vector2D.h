#pragma once
#include<iostream>
using namespace std;
class Vector2D {
public:
	Vector2D(double i_x, double i_y);
	Vector2D();
	//setter
	inline void set_x(double i_x);
	inline void set_y(double i_y);
	//getter
	inline double get_x() const;
	inline double get_y() const;
	
	inline void normalize();
	inline double getMagnitude();
//private:
	double x, y;
};

inline Vector2D operator+(const Vector2D& l_value, const Vector2D& r_value);
inline Vector2D operator-(const Vector2D& l_value, const Vector2D& r_value);
inline ostream & operator<<(ostream &out, const Vector2D &obj);
inline istream & operator >> (istream &in, Vector2D &obj);
inline Vector2D operator*(const Vector2D& l_value, float r_value);

#include "Vector2D-inl.h"