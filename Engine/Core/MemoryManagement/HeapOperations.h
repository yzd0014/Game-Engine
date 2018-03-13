#pragma once
#include "HeapManager.h"
void * operator new(size_t i_size);
void operator delete(void * i_ptr);
void *operator new[](size_t i_size);
void operator delete[](void * i_ptr);
void *operator new(size_t i_size, HeapManager* i_heapManager, unsigned int i_aligment);
void operator delete(void * i_ptr, HeapManager* i_heapManager, unsigned int i_aligment);