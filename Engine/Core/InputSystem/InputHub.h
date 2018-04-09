#pragma once
#include "Core\Messaging\Messaging.h"
#include "Core\DataType\Delegate.h"

namespace Engine {
	namespace InputHub {
		extern Messaging * inputTrigger;
		bool Init();
	} // namespace Input
} // namespace Engine
