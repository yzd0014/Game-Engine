#pragma once
#include "HeapManager.h"
#include "iostream"
#include "Core\Utility\Utility.h"

using namespace std;
inline HeapManager * HeapManager::create(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors) {
	uintptr_t pTrueStarter;
		pTrueStarter = reinterpret_cast<uintptr_t>(i_pMemory) + sizeof(HeapManager);
	if (heapManager == nullptr) {
		heapManager = new (i_pMemory) HeapManager(reinterpret_cast<void *>(pTrueStarter), i_sizeMemory - sizeof(HeapManager), i_numDescriptors);
	}
	
	return heapManager;
}

inline void * HeapManager::_alloc(size_t i_size) {
	return _alloc(i_size, 4);
}

inline void HeapManager::destroy()const {
	if (outstandingHeader.next != NULL) {
		//cout << "Unsafe operation, there are still outstanding allocations." << endl;
		DEBUG_PRINT_INFO("Unsafe operation, there are still outstanding allocations.");
	}
	else {
		//cout << "HeapManager is destroyed successfully." << endl;
		DEBUG_PRINT_INFO("HeapManager is destroyed successfully.");
	}

}

inline void  HeapManager::showFreeBlocks()const {
	BlockDescriptor * iterator = freeHeader.next;
	cout << "Free blocks: " << endl;
	while (iterator != NULL) {
		cout << "Start address: " << iterator->starter << endl;
		cout << "Size: " << iterator->size << endl;
		if (iterator->next == NULL) {
			cout << "Next BlokDescriptor's address: " << "0" << "\n" << endl;
		}
		else {
			cout << "Next BlokDescriptor's address: " << iterator->next << "\n" << endl;
		}
		iterator = iterator->next;
	}
}

inline bool HeapManager::contains(void * i_ptr)const
{
	bool output = false;
	if (reinterpret_cast<uintptr_t>(i_ptr) >= reinterpret_cast<uintptr_t>(heapStarter)
		&& reinterpret_cast<uintptr_t>(i_ptr) < reinterpret_cast<uintptr_t>(heapStarter) + heapSize) {
		output = true;
	}
	//assert(output);
	return output;
}

inline bool HeapManager::isAllocated(void * i_ptr)const
{
	bool output = false;
	BlockDescriptor * iterator = outstandingHeader.next;
	while (iterator != NULL) {
		if (reinterpret_cast<uintptr_t>(i_ptr) < reinterpret_cast<uintptr_t>(iterator->starter) + iterator->size
			&& reinterpret_cast<uintptr_t>(i_ptr) >= reinterpret_cast<uintptr_t>(iterator->starter)) {
		//if ((size_t)i_ptr < (size_t)iterator->starter + iterator->size && (size_t)i_ptr >= (size_t)iterator->starter) {
			output = true;
			break;
		}
		iterator = iterator->next;
	}
	//assert(output);
	return output;
}

inline size_t HeapManager::getLargestFreeBlock()const
{
	size_t largest = 0;
	BlockDescriptor * iterator = freeHeader.next;
	while (iterator != NULL) {
		if (iterator->size > largest) {
			largest = iterator->size;
		}
		iterator = iterator->next;
	}
	return largest;
}

inline size_t HeapManager::getTotalFreeMemory()const
{
	size_t sum = 0;
	BlockDescriptor * iterator = freeHeader.next;
	while (iterator != NULL) {
		sum = sum + iterator->size;
		iterator = iterator->next;
	}
	return sum;
}

inline void HeapManager::showOutstandingAllocations()const {
	BlockDescriptor * iterator = outstandingHeader.next;
	cout << "Outsanding blocks: " << endl;
	while (iterator != NULL) {
		cout << "Start address: " << iterator->starter << endl;
		cout << "Size: " << iterator->size << endl;
		if (iterator->next == NULL) {
			cout << "Next BlokDescriptor's address: " << "0" << "\n" << endl;
		}
		else {
			cout << "Next BlokDescriptor's address: " << iterator->next << "\n" << endl;
		}
		iterator = iterator->next;
	}
}

inline unsigned int HeapManager::getTotalNumDescriptors() const {
	BlockDescriptor * iterator = unusedHeader.next;
	unsigned int numDescriptors = 0;
	while (iterator != NULL) {
		numDescriptors++;
		iterator = iterator->next;
	}
	return numDescriptors;
}

inline void HeapManager::collect() {
	mtx.lock();
	BlockDescriptor * iterator = freeHeader.next;
	while (iterator != NULL && iterator->next != NULL) {
		if (reinterpret_cast<uintptr_t>(iterator->starter) + iterator->size == reinterpret_cast<uintptr_t>(iterator->next->starter)) {
		//if ((size_t)iterator->starter + iterator->size == (size_t)iterator->next->starter) {
			
			iterator->size = iterator->size + iterator->next->size;
			//iterator->size = (size_t)iterator->size + (size_t)iterator->next->size;

			BlockDescriptor * temp = iterator->next->next;

			iterator->next->starter = NULL;
			iterator->next->size = 0;
			iterator->next->next = unusedHeader.next;
			unusedHeader.next = iterator->next;
			iterator->next = temp;
			continue;
		}
		iterator = iterator->next;
	}
	mtx.unlock();
}

inline void  HeapManager::sortInsert(BlockDescriptor * i_ptr) {
	bool found = false;
	BlockDescriptor * iterator = &freeHeader;
	while (iterator->next != NULL) {
		if (iterator->next->starter > i_ptr->starter) {
			i_ptr->next = iterator->next;
			iterator->next = i_ptr;
			found = true;
			break;
		}
		iterator = iterator->next;
	}

	if (found == false) {
		i_ptr->next = iterator->next;
		iterator->next = i_ptr;
	}
}