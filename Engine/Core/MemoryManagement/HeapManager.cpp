#include "HeapManager.h"
#include <iostream>
#include <assert.h>

using namespace std;
HeapManager * HeapManager::heapManager = nullptr;

HeapManager::HeapManager(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors) {
	heapStarter = i_pMemory;
	heapSize = i_sizeMemory;

	int descriptorSize = sizeof(BlockDescriptor);
	
	freeHeader.next = reinterpret_cast<BlockDescriptor *>(reinterpret_cast<uintptr_t>(i_pMemory) + i_sizeMemory - i_numDescriptors * descriptorSize);
	//freeHeader.next = (BlockDescriptor *)((size_t)i_pMemory + i_sizeMemory - i_numDescriptors * descriptorSize);
	
	freeHeader.next->starter = i_pMemory;
	assert(i_sizeMemory > i_numDescriptors * descriptorSize);
	freeHeader.next->size = i_sizeMemory - i_numDescriptors * descriptorSize;
	freeHeader.next->next = NULL;

	outstandingHeader.next = NULL;

	unusedHeader.next = NULL;
	BlockDescriptor * newDescriptor = freeHeader.next + 1;
	for (unsigned int i = 0; i < i_numDescriptors - 1; i++) {
		newDescriptor->starter = NULL;
		newDescriptor->size = 0;
		newDescriptor->next = unusedHeader.next;
		unusedHeader.next = newDescriptor;
		newDescriptor = newDescriptor + 1;
	}
}


void * HeapManager::_alloc(size_t i_size, unsigned int i_alignment)
{
	mtx.lock();
	uintptr_t output = NULL;
	//size_t output = NULL;
	
	BlockDescriptor * iterator = &freeHeader;
	while (iterator->next != NULL && unusedHeader.next != NULL) {
		if (iterator->next->size >= i_size + 8) {
			output = reinterpret_cast<uintptr_t>(iterator->next->starter) + iterator->next->size - i_size - 4;
			//output = (size_t)iterator->next->starter + iterator->next->size - i_size;
			
			uintptr_t remainder = output % i_alignment;
			//size_t remainder = output % (size_t)i_alignment;
			
			//output = output - remainder;
			//iterator->next->size = iterator->next->size - i_size - remainder;

			if (iterator->next->size == i_size + remainder + 8) {//talk out from free list and add descriptor back to unused list
				output = output - remainder;
				iterator->next->size = iterator->next->size - i_size - remainder - 8;

				BlockDescriptor * temp = iterator->next->next;
				iterator->next->starter = NULL;
				iterator->next->size = 0;
				iterator->next->next = unusedHeader.next;
				unusedHeader.next = iterator->next;
				iterator->next = temp;
			}
			else if (iterator->next->size < i_size + remainder + 8) {
				output = NULL;
				iterator = iterator->next;
				continue;
			}
			else {
				output = output - remainder;
				iterator->next->size = iterator->next->size - i_size - remainder - 8;
			}
			unusedHeader.next->starter = reinterpret_cast<void *>(output - 4);
			//unusedHeader.next->starter = (void *)output;
			
			unusedHeader.next->size = i_size + remainder + 8;
			BlockDescriptor * temp = unusedHeader.next->next;
			unusedHeader.next->next = outstandingHeader.next;
			outstandingHeader.next = unusedHeader.next;
			unusedHeader.next = temp;
			break;
		}
		iterator = iterator->next;
	}
	mtx.unlock();
	return reinterpret_cast<void *>(output);
	//return (void *)output;
}

bool HeapManager::_free(void * i_ptr) {
	mtx.lock();
	bool freed = false;
	i_ptr = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(i_ptr) - 4);
	BlockDescriptor * iterator = &outstandingHeader;
	while (iterator->next != NULL) {
		if (iterator->next->starter == i_ptr) {
			BlockDescriptor * temp = iterator->next->next;
			sortInsert(iterator->next);
			iterator->next = temp;
			freed = true;
			break;
		}
		iterator = iterator->next;
	}
	mtx.unlock();
	return freed;
}












