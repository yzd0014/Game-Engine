#include<stdio.h>
#include <stdlib.h>  
#include <stdarg.h>
#include <Windows.h>
#include "Core\MemoryManagement\HeapOperations.h"

namespace Engine
{
	
	void ConsolePrint(const char * i_fmt, ...) {
		const size_t lenTemp = 256;
		char strTemp[lenTemp] = "Debug: ";
		strcat_s(strTemp, i_fmt);
		strcat_s(strTemp, "\n");

		const size_t lenOutput = lenTemp + 32;

		char strOutput[lenOutput];

		va_list args;
		va_start(args, i_fmt);

		vsprintf_s(strOutput, lenOutput, strTemp, args);
		va_end(args);
		OutputDebugStringA(strOutput);

	}
	char * duplicateString(const char * i_string) {
		size_t len = strlen(i_string);
		char * string = new char[len + 1];
		strcpy_s(string, len + 1, i_string);
		return string;
	}
	bool floatEqual(float i_left, float i_right) {
		return abs(i_left - i_right) < 0.001f;
	}
	
}