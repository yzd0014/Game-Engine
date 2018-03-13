#pragma once
#include "Vector2D.h"

inline void Vector2D::set_x(double i_x) {
	x = i_x;
}

inline void Vector2D::set_y(double i_y) {
	y = i_y;
}

inline double Vector2D::get_x() const{
	return x;
}
inline double Vector2D::get_y() const{
	return y;
}
inline void  Vector2D::normalize() {
	double magnitude = getMagnitude();
	x = x / magnitude;
	y = y / magnitude;
}

inline double Vector2D::getMagnitude() {
	double magnitude;
	magnitude = sqrt (x * x + y * y);
	return magnitude;
}

inline Vector2D operator+(const Vector2D& l_value, const Vector2D& r_value) {
	Vector2D output(0, 0);
	output.set_x(l_value.get_x() + r_value.get_x());
	output.set_y(l_value.get_y() + r_value.get_y());

	return output;
}

inline Vector2D operator-(const Vector2D& l_value, const Vector2D& r_value) {
	Vector2D output(0, 0);
	output.set_x(l_value.get_x() - r_value.get_x());
	output.set_y(l_value.get_y() - r_value.get_y());

	return output;
}
inline Vector2D operator*(const Vector2D& l_value, float r_value) {
	return Vector2D(l_value.get_x() * r_value, l_value.get_y() * r_value);
}

inline ostream & operator<<(ostream &out, const Vector2D &obj) {
	out << "[" << obj.get_x() << ", " << obj.get_y() << "]";
	return out;
}
inline istream & operator >> (istream &in, Vector2D &obj) {
	double x = 0.0, y = 0.0;
	in >> x >> y;
	obj.set_x(x);
	obj.set_y(y);
	return in;
}