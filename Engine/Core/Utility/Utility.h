#pragma once
namespace Engine {
	void ConsolePrint(const char * i_fmt, ...);
	char * duplicateString(const char * i_string);
	bool floatEqual(float i_left, float i_right);
}

#if defined(_DEBUG)  &&  !defined(DISABLE_DEBUG_PRINT)
#define DEBUG_PRINT_INFO(fmt, ...) Engine::ConsolePrint((fmt), __VA_ARGS__)
#else
#define DEBUG_PRINT_INFO(fmt, ...) void(0)
#endif

#define dupstring(str) Engine::duplicateString(str)