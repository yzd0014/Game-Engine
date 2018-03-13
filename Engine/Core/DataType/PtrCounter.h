#pragma once
struct PtrCounter {
	long refCounter;
	long weakCounter;
	PtrCounter(long i_refCounter, long i_weakCounter) :
		refCounter(i_refCounter),
		weakCounter(i_weakCounter) {

	}
};