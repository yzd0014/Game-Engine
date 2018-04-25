#pragma once
#include "Vector4D.h"
#include <smmintrin.h>
#include "MathMacro.h"
class Matrix4x4 {
public:
	Matrix4x4(float * i_Value);
	Matrix4x4();

	float getDetermiant();
	Matrix4x4 getCofactors();
	Matrix4x4 getTranspose() const;
	Matrix4x4 getInverse();

	Matrix4x4 operator*(const Matrix4x4 & i_other);
	Vector4D operator*(const Vector4D & i_vector);
	
	static Matrix4x4 CreateXRotation(float i_degree);
	static Matrix4x4 CreateYRotation(float i_degree);
	static Matrix4x4 CreateZRotation(float i_degree);
	static Matrix4x4 CreateScale(float i_X, float i_Y, float i_Z);
	static Matrix4x4 CreateTranslation(float i_X, float i_Y, float i_Z);
	union {
		float m_Value[16];
		__m128 rows[4];
	};
private:
	float getDetermiantX3(float * i_value);
	float getDetermiantX2(float * i_value);
	

};

 