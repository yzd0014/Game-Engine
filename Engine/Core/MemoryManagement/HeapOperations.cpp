#include "HeapManager.h"
#include "HeapOperations.h"
#include "FixedSizeAllocator.h"

void * operator new(size_t i_size) {
	void * pOutput;
	if (i_size <= 8) {
		pOutput = FixedSizeAllocator::FSAs[0]->_alloc();
		if (pOutput != nullptr) {
			DEBUG_PRINT_INFO("8 byte allocated from FSA");
			return pOutput;
		}
	}
	if (i_size <= 16) {
		pOutput = FixedSizeAllocator::FSAs[1]->_alloc();
		if (pOutput != nullptr) {
			DEBUG_PRINT_INFO("16 byte allocated from FSA");
			return pOutput;
		}
	}
	if (i_size <= 32){
		pOutput = FixedSizeAllocator::FSAs[2]->_alloc();
		if (pOutput != nullptr) {
			DEBUG_PRINT_INFO("32 byte allocated from FSA");
			return pOutput;
		}
	}
	if (i_size <= 64) {
		pOutput = FixedSizeAllocator::FSAs[3]->_alloc();
		if (pOutput != nullptr) {
			DEBUG_PRINT_INFO("64 byte allocated from FSA");
			return pOutput;
		}
	}
	DEBUG_PRINT_INFO("default/ flexible allocater is called");
	return HeapManager::heapManager->_alloc(i_size);
}

void operator delete(void * i_ptr) {
	if (FixedSizeAllocator::FSAs[0] != nullptr && FixedSizeAllocator::FSAs[0]->contains(i_ptr)) {
		FixedSizeAllocator::FSAs[0]->_free(i_ptr);
	}
	else if (FixedSizeAllocator::FSAs[1] != nullptr && FixedSizeAllocator::FSAs[1]->contains(i_ptr)) {
		FixedSizeAllocator::FSAs[1]->_free(i_ptr);
	}
	else if (FixedSizeAllocator::FSAs[2] != nullptr && FixedSizeAllocator::FSAs[2]->contains(i_ptr)) {
		FixedSizeAllocator::FSAs[2]->_free(i_ptr);
	}
	else if (FixedSizeAllocator::FSAs[3] != nullptr && FixedSizeAllocator::FSAs[3]->contains(i_ptr)) {
		FixedSizeAllocator::FSAs[3]->_free(i_ptr);
	}
	else {
		HeapManager::heapManager->_free(i_ptr);
	}
}

void *operator new[](size_t i_size) {
	void * pOutput;
	if (i_size <= 8) {
		pOutput = FixedSizeAllocator::FSAs[0]->_alloc();
		if (pOutput != nullptr) {
			DEBUG_PRINT_INFO("8 byte allocated from FSA");
			return pOutput;
		}
	}
	if (i_size <= 16) {
		pOutput = FixedSizeAllocator::FSAs[1]->_alloc();
		if (pOutput != nullptr) {
			DEBUG_PRINT_INFO("16 byte allocated from FSA");
			return pOutput;
		}
	}
	if (i_size <= 32) {
		pOutput = FixedSizeAllocator::FSAs[2]->_alloc();
		if (pOutput != nullptr) {
			DEBUG_PRINT_INFO("32 byte allocated from FSA");
			return pOutput;
		}
	}
	if (i_size <= 64) {
		pOutput = FixedSizeAllocator::FSAs[3]->_alloc();
		if (pOutput != nullptr) {
			DEBUG_PRINT_INFO("64 byte allocated from FSA");
			return pOutput;
		}
	}
	DEBUG_PRINT_INFO("default heap is used");
	return HeapManager::heapManager->_alloc(i_size);
}

void operator delete[](void * i_ptr) {
	if (FixedSizeAllocator::FSAs[0] != nullptr && FixedSizeAllocator::FSAs[0]->contains(i_ptr)) {
		FixedSizeAllocator::FSAs[0]->_free(i_ptr);
	}
	else if (FixedSizeAllocator::FSAs[1] != nullptr && FixedSizeAllocator::FSAs[1]->contains(i_ptr)) {
		FixedSizeAllocator::FSAs[1]->_free(i_ptr);
	}
	else if (FixedSizeAllocator::FSAs[2] != nullptr && FixedSizeAllocator::FSAs[2]->contains(i_ptr)) {
		FixedSizeAllocator::FSAs[2]->_free(i_ptr);
	}
	else if (FixedSizeAllocator::FSAs[3] != nullptr && FixedSizeAllocator::FSAs[3]->contains(i_ptr)) {
		FixedSizeAllocator::FSAs[3]->_free(i_ptr);
	}
	else {
		HeapManager::heapManager->_free(i_ptr);
	}
}

void *operator new(size_t i_size, HeapManager* i_heapManager, unsigned int i_aligment) {
	return i_heapManager->_alloc(i_size, i_aligment);
}

void operator delete(void * i_ptr, HeapManager* i_heapManager, unsigned int i_aligment) {
	i_heapManager->_free(i_ptr);
}