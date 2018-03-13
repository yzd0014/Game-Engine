#pragma once
struct BlockDescriptor {
	void * starter;
	size_t size;
	BlockDescriptor * next;
};
