#include"Engine.h"
bool Matrix_StressUnitTest() {
	PROFILE_UNSCOPED(matrix_StressUnitTest);

	float a1[16] = { 1, 0, 0, 0,
		0, 40, 41, 0,
		0, 42, 43, 0,
		0, 0, 0, 1 };
	Matrix4x4 m1(a1);
	Matrix4x4 r_m;
	Vector4D vec1(1, 2, 3, 4);
	Vector4D r_v;
	
	for (int i = 0; i < 1000000; i++) {
		//r_v = m1 * vec1;
		r_m = m1 * m1;
	}
	
	PROFILING_PRINT_INFO("%f", r_m.m_Value[0]);
	//PROFILING_PRINT_INFO("%f", r_v.x);
	return true;
}