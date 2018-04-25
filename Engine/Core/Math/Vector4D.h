#pragma once
#include "math.h"
#include "assert.h"
#include <smmintrin.h>
#include "MathMacro.h"

class Vector4D {
public:
	Vector4D();
	Vector4D(float i_x, float i_y, float i_z, float i_w);
	inline float getMagnitude() const {
		return (float)sqrt(x * x + y * y + z * z);
	}
	inline void normalize() {
		float length = getMagnitude();
		assert(length != 0);
		x = x / length;
		y = y / length;
		z = z / length;
	}
	
	union {
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		__m128 m_vec;
	};
};

inline Vector4D operator+(const Vector4D& l_value, const Vector4D& r_value) {
	return Vector4D(l_value.x + r_value.x, l_value.y + r_value.y, l_value.z + r_value.z, 1);
}
inline Vector4D operator-(const Vector4D& l_value, const Vector4D& r_value) {
	return Vector4D(l_value.x + r_value.x, l_value.y + r_value.y, l_value.z + r_value.z, 1);
}
inline Vector4D operator*(const Vector4D& l_value, float r_value) {
	return Vector4D(l_value.x * r_value, l_value.y * r_value, l_value.z * r_value, l_value.w);
}

inline Vector4D operator*(float l_value, const Vector4D& r_value) {
	return Vector4D(r_value.x * l_value, r_value.y * l_value, r_value.z * l_value, r_value.w);
}
inline float dot(const Vector4D &l_value, const Vector4D &r_value) {
#if defined SIMD
	__m128 r = _mm_dp_ps(l_value.m_vec, r_value.m_vec, 0xf1);
	float dot = _mm_cvtss_f32(r);
	return dot;
#else
	return l_value.x * r_value.x + l_value.y * r_value.y + l_value.z * r_value.z + l_value.w * r_value.w;
#endif
}