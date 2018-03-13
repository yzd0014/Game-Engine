#include "Engine.h"
#include "assert.h"
bool Matrix_UnitTest() {
	float a1[16] = {1, 0, 0, 0,
				   0, 40, 41, 0,
				   0, 42, 43, 0,
				   0, 0, 0, 1};
	
	Matrix4x4 m1(a1);
	Matrix4x4 m1_Inverse;

	//test inverse matrix
	m1_Inverse = m1.getInverse();
	
	float a_Expected[16] = { 1, 0, 0, 0,
							0, -21.5, 20.5, 0,
							0, 21, -20, 0,
							0, 0, 0, 1 };
	for (int i = 0; i < 16; i++) {
		assert(Engine::floatEqual(m1_Inverse.m_Value[i], a_Expected[i]));
	}

	//test matrix mutiplies matrix
	a1[0] = 1; a1[1] = 1; a1[2] = 1; a1[3] = 1;
	a1[4] = 2; a1[5] = 2; a1[6] = 2; a1[7] = 2;
	a1[8] = 3; a1[9] = 3; a1[10] = 3; a1[11] = 3;
	a1[12] = 4; a1[13] = 4; a1[14] = 4; a1[15] = 4;
	Matrix4x4 m2(a1);

	a1[0] = 1; a1[1] = 2; a1[2] = 3; a1[3] = 4;
	a1[4] = 5; a1[5] = 6; a1[6] = 7; a1[7] = 8;
	a1[8] = 9; a1[9] = 10; a1[10] = 11; a1[11] = 12;
	a1[12] = 13; a1[13] = 14; a1[14] = 15; a1[15] = 16;
	Matrix4x4 m3(a1);
	Matrix4x4 m4 = m2 * m3;
	float a2_Expected[16] = { 28, 32, 36, 40,
						56, 64, 72, 80,
						84, 96, 108, 120,
						112, 128, 144, 160 };

	for (int i = 0; i < 16; i++) {
		assert(Engine::floatEqual(m4.m_Value[i], a2_Expected[i]));
	}
	
	//test matrix mutiplies vector
	Vector4D v1(1, 2, 3, 4);
	v1 = m2 * v1;
	assert(Engine::floatEqual(v1.x, 10));
	assert(Engine::floatEqual(v1.y, 20));
	assert(Engine::floatEqual(v1.z, 30));
	assert(Engine::floatEqual(v1.w, 40));

	//test rotation matrix
	Vector4D vZ(1, 0, 0, 1);
	Matrix4x4 rotZ = Matrix4x4::CreateZRotation(90);
	vZ = rotZ * vZ;
	assert(Engine::floatEqual(vZ.x, 0));
	assert(Engine::floatEqual(vZ.y, 1));
	assert(Engine::floatEqual(vZ.z, 0));
	assert(Engine::floatEqual(vZ.w, 1));

	Vector4D vY(0, 0, 1, 1);
	Matrix4x4 rotY = Matrix4x4::CreateYRotation(90);
	vY = rotY * vY;
	assert(Engine::floatEqual(vY.x, 1));
	assert(Engine::floatEqual(vY.y, 0));
	assert(Engine::floatEqual(vY.z, 0));
	assert(Engine::floatEqual(vY.w, 1));

	Vector4D vX(0, 1, 0, 1);
	Matrix4x4 rotX = Matrix4x4::CreateXRotation(90);
	vX = rotX * vX;
	assert(Engine::floatEqual(vX.x, 0));
	assert(Engine::floatEqual(vX.y, 0));
	assert(Engine::floatEqual(vX.z, 1));
	assert(Engine::floatEqual(vX.w, 1));

	//test scale
	Vector4D v_Scale(1, 1, 1, 1);
	Matrix4x4 m_Scale = Matrix4x4::CreateScale(1, 2, 3);
	v_Scale = m_Scale * v_Scale;
	assert(Engine::floatEqual(v_Scale.x, 1));
	assert(Engine::floatEqual(v_Scale.y, 2));
	assert(Engine::floatEqual(v_Scale.z, 3));
	assert(Engine::floatEqual(v_Scale.w, 1));

	//test translation
	Vector4D v_Translate(1, 1, 1, 1);
	Matrix4x4 m_Translate = Matrix4x4::CreateTranslation(1, 1, 1);
	v_Translate = m_Translate * v_Translate;
	assert(Engine::floatEqual(v_Translate.x, 2));
	assert(Engine::floatEqual(v_Translate.y, 2));
	assert(Engine::floatEqual(v_Translate.z, 2));
	assert(Engine::floatEqual(v_Translate.w, 1));

	//comprehensive test
	Vector4D v_Start(1, 0, 0, 1);
	rotZ = Matrix4x4::CreateZRotation(-45);
	m_Translate = Matrix4x4::CreateTranslation(1, 1, 1);
	m_Scale = Matrix4x4::CreateScale(2, 2, 2);
	Matrix4x4 m_Transform = m_Scale * m_Translate * rotZ;
	Vector4D v_End = m_Transform * v_Start;

	m_Transform = m_Transform.getInverse();
	v_End = m_Transform * v_End;
	assert(Engine::floatEqual(v_Start.x, v_End.x));
	assert(Engine::floatEqual(v_Start.y, v_End.y));
	assert(Engine::floatEqual(v_Start.z, v_End.z));
	assert(Engine::floatEqual(v_Start.w, v_End.w));
	
	return true;
}