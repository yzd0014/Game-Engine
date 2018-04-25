#include "Matrix4x4.h"
#include "Vector4D.h"
#include "Core\MemoryManagement\HeapOperations.h"
#include "MathMacro.h"

#include "assert.h"
#include <smmintrin.h>

#define PI 3.14159265

Matrix4x4::Matrix4x4(float * i_Value) {
	for (int i = 0; i < 16; i++) {
		m_Value[i] = i_Value[i];
	}
}

Matrix4x4::Matrix4x4() {
	for (int i = 0; i < 16; i++) {
		m_Value[i] = 0;
	}
	m_Value[0] = 1;
	m_Value[5] = 1;
	m_Value[10] = 1;
	m_Value[15] = 1;
}
float Matrix4x4::getDetermiant() {
	int j = 0;//iterator for result/output array

	float A1[9];
	
	for (int i = 0; i < 16; i++) {
		if (i != 0 && i != 1 && i != 2 && i != 3 && i != 4 && i != 8 && i != 12) {
			A1[j] = m_Value[i];
			j++;
		}
	}
	float temp_1 = m_Value[0] * getDetermiantX3(A1);

	
	j = 0;
	float A2[9];
	for (int i = 0; i < 16; i++) {
		if (i != 0 && i != 1 && i != 2 && i != 3 && i != 5 && i != 9 && i != 13) {
			A2[j] = m_Value[i];
			j++;
		}
	}
	float temp_2 = m_Value[1] * getDetermiantX3(A2);

	j = 0;
	float A3[9];
	for (int i = 0; i < 16; i++) {
		if (i != 0 && i != 1 && i != 2 && i != 3 && i != 6 && i != 10 && i != 14) {
			A3[j] = m_Value[i];
			j++;
		}
	}
	float temp_3 = m_Value[2] * getDetermiantX3(A3);

	j = 0;
	float A4[9];
	for (int i = 0; i < 16; i++) {
		if (i != 0 && i != 1 && i != 2 && i != 3 && i != 7 && i != 11 && i != 15) {
			A4[j] = m_Value[i];
			j++;
		}
	}
	float temp_4 = m_Value[3] * getDetermiantX3(A4);

	return temp_1-temp_2 + temp_3 - temp_4;
}


float Matrix4x4::getDetermiantX3(float * i_value) {
	float A1[4] = {i_value[4], i_value[5], i_value[7], i_value[8]};
	float temp_1 = i_value[0] * getDetermiantX2(A1);
	
	float A2[4] = {i_value[3], i_value[5], i_value[6], i_value[8]};
	float temp_2 = i_value[1] * getDetermiantX2(A2);

	float A3[4] = { i_value[3], i_value[4], i_value[6], i_value[7] };
	float temp_3 = i_value[2] * getDetermiantX2(A3);

	return temp_1 - temp_2 + temp_3;
}

float Matrix4x4::getDetermiantX2(float * i_value) {
	return i_value[0] * i_value[3] - i_value[1] * i_value[2];
}

Matrix4x4 Matrix4x4::getCofactors() {
	Matrix4x4 output;

	for (int i = 0; i < 16; i++) {
		int row = i / 4;
		int column = i % 4;

		int c = 0;//counter for internal array
		float interalArray[9];
		for (int j = 0; j < 16; j++) {
			int innerRow = j / 4;
			int innerColumn = j % 4;
			if (innerRow != row && innerColumn != column) {
				interalArray[c] = m_Value[j];
				c++;
			}
		}
		output.m_Value[i] = getDetermiantX3(interalArray) * (float)pow(-1, row + column);
	}

	return output;
}

Matrix4x4 Matrix4x4::getTranspose() const{
#if defined SIMD
	Matrix4x4 output;
	for (int i = 0; i < 4; i++) {
		output.rows[i] = _mm_set_ps(m_Value[i + 12], m_Value[i+8], m_Value[i+4], m_Value[i]);
	}
	return output;
#else
	Matrix4x4 output;
	int i = 0;
	int j = 0;
	while (j < 16) {
		output.m_Value[j] = m_Value[i];
		j++;
		i = i + 4;
		if (i > 15)i = i - 15;
	}
	
	return output;
#endif
}

Matrix4x4 Matrix4x4::getInverse() {
	Matrix4x4 output = getCofactors();
	Matrix4x4 temp = output;
	
	//get transpose matrix
#if defined SIMD
	for (int i = 0; i < 4; i++) {
		output.rows[i] = _mm_set_ps(temp.m_Value[i + 12], temp.m_Value[i + 8], temp.m_Value[i + 4], temp.m_Value[i]);
}
#else
	int i = 0;
	int j = 0;
	while (j < 16) {
		output.m_Value[j] = temp.m_Value[i];
		j++;
		i = i + 4;
		if (i > 15)i = i - 15;
	}
#endif	
	
	float m_Determiant = getDetermiant();
	assert(m_Determiant != 0);
	for (int i = 0; i < 16; i++) {
		output.m_Value[i] = (1 / m_Determiant)*output.m_Value[i];
	}
	return output;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 & i_other) {
#if defined SIMD
	Matrix4x4 output;
	Matrix4x4 a_transpose = getTranspose();
	Matrix4x4 b_transpose = i_other.getTranspose();
	//__m128 a_line, b_line, r_line;
	__m128 b_line, r_line;
	for (int i = 0; i < 16; i+=4) {
		//a_line = _mm_load_ps(a_transpose.m_Value);
		b_line = _mm_set1_ps(b_transpose.m_Value[i]);
		r_line = _mm_mul_ps(a_transpose.rows[0], b_line);

		b_line = _mm_set1_ps(b_transpose.m_Value[i + 1]);
		r_line = _mm_add_ps(_mm_mul_ps(a_transpose.rows[1], b_line), r_line);

		b_line = _mm_set1_ps(b_transpose.m_Value[i + 2]);
		r_line = _mm_add_ps(_mm_mul_ps(a_transpose.rows[2], b_line), r_line);

		b_line = _mm_set1_ps(b_transpose.m_Value[i + 3]);
		r_line = _mm_add_ps(_mm_mul_ps(a_transpose.rows[3], b_line), r_line);
		_mm_store_ps(&output.m_Value[i], r_line);
	}
	output = output.getTranspose();
	return output;

#else
	Matrix4x4 output;
	for (int i = 0; i < 4; i++) {
		output.m_Value[i] = m_Value[0] * i_other.m_Value[i] + m_Value[1] * i_other.m_Value[4 + i] + m_Value[2] * i_other.m_Value[8 + i] + m_Value[3] * i_other.m_Value[12 + i];
		output.m_Value[4 + i] = m_Value[4] * i_other.m_Value[i] + m_Value[5] * i_other.m_Value[4 + i] + m_Value[6] * i_other.m_Value[8 + i] + m_Value[7] * i_other.m_Value[12 + i];
		output.m_Value[8 + i] = m_Value[8] * i_other.m_Value[i] + m_Value[9] * i_other.m_Value[4 + i] + m_Value[10] * i_other.m_Value[8 + i] + m_Value[11] * i_other.m_Value[12 + i];
		output.m_Value[12 + i] = m_Value[12] * i_other.m_Value[i] + m_Value[13] * i_other.m_Value[4 + i] + m_Value[14] * i_other.m_Value[8 + i] + m_Value[15] * i_other.m_Value[12 + i];
	}
	return output;
#endif
}

Vector4D Matrix4x4::operator*(const Vector4D & i_vector) {
	Vector4D output;
#if defined SIMD	
	__m128 r = _mm_dp_ps(rows[0], i_vector.m_vec, 0xf1);
	output.x = _mm_cvtss_f32(r);

	r = _mm_dp_ps(rows[1], i_vector.m_vec, 0xf1);
	output.y = _mm_cvtss_f32(r);

	r = _mm_dp_ps(rows[2], i_vector.m_vec, 0xf1);
	output.z = _mm_cvtss_f32(r);

	r = _mm_dp_ps(rows[3], i_vector.m_vec, 0xf1);
	output.w = _mm_cvtss_f32(r);

#else
	output.x = m_Value[0] * i_vector.x + m_Value[1] * i_vector.y + m_Value[2] * i_vector.z + m_Value[3] * i_vector.w;
	output.y = m_Value[4] * i_vector.x + m_Value[5] * i_vector.y + m_Value[6] * i_vector.z + m_Value[7] * i_vector.w;
	output.z = m_Value[8] * i_vector.x + m_Value[9] * i_vector.y + m_Value[10] * i_vector.z + m_Value[11] * i_vector.w;
	output.w = m_Value[12] * i_vector.x + m_Value[13] * i_vector.y + m_Value[14] * i_vector.z + m_Value[15] * i_vector.w;
#endif
	return output;
}

Matrix4x4 Matrix4x4::CreateXRotation(float i_degree) {
	Matrix4x4 output;
	float angle = (float)(PI *i_degree / 180);
	for (int i = 0; i < 16; i++) {
		output.m_Value[i] = 0;
	}
	output.m_Value[0] = 1;
	output.m_Value[5] = cos(angle);
	output.m_Value[6] = -1 * sin(angle);
	output.m_Value[9] = sin(angle);
	output.m_Value[10] = cos(angle);
	output.m_Value[15] = 1;
	
	return output;
}

Matrix4x4 Matrix4x4::CreateYRotation(float i_degree) {
	Matrix4x4 output;
	float angle = (float)(PI *i_degree / 180);
	for (int i = 0; i < 16; i++) {
		output.m_Value[i] = 0;
	}
	output.m_Value[0] = cos(angle);
	output.m_Value[3] = sin(angle);
	output.m_Value[5] = 1;
	output.m_Value[8] = -1 * sin(angle);
	output.m_Value[10] = cos(angle);
	output.m_Value[15] = 1;

	return output;
}

Matrix4x4 Matrix4x4::CreateZRotation(float i_degree) {
	Matrix4x4 output;
	float angle = (float)(PI *i_degree / 180);
	for (int i = 0; i < 16; i++) {
		output.m_Value[i] = 0;
	}
	output.m_Value[0] = cos(angle);
	output.m_Value[1] = -1 * sin(angle);
	output.m_Value[4] = sin(angle);
	output.m_Value[5] = cos(angle);
	output.m_Value[10] = 1;
	output.m_Value[15] = 1;

	return output;
}

Matrix4x4 Matrix4x4::CreateScale(float i_X, float i_Y, float i_Z) {
	Matrix4x4 output;
	for (int i = 0; i < 16; i++) {
		output.m_Value[i] = 0;
	}
	output.m_Value[0] = i_X;
	output.m_Value[5] = i_Y;
	output.m_Value[10] = i_Z;
	output.m_Value[15] = 1;
	
	return output;
}

Matrix4x4 Matrix4x4::CreateTranslation(float i_X, float i_Y, float i_Z) {
	Matrix4x4 output;
	for (int i = 0; i < 16; i++) {
		output.m_Value[i] = 0;
	}

	output.m_Value[0] = 1;
	output.m_Value[3] = i_X;
	output.m_Value[5] = 1;
	output.m_Value[7] = i_Y;
	output.m_Value[10] = 1;
	output.m_Value[11] = i_Z;
	output.m_Value[15] = 1;

	return output;
}