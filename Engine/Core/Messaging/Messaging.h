#pragma once
#include <map>
#include <vector>
#include <algorithm>
#include "assert.h"

#include "Core\DataType\Delegate.h"

class Messaging {
public:
	Messaging() {}
	MultiCastDelegate<unsigned int, bool> ButtonChangeReceivers;
};
