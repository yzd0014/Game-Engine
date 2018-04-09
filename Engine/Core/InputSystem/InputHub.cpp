#include "assert.h"
#include "InputHub.h"
#include "EngineLow\Rendering\GLib\GLib.h"

namespace Engine {
	namespace InputHub {
		Messaging * inputTrigger = nullptr;
		void ButtonChangeCallback(unsigned int i_VKey, bool bWentDown)
		{
			assert(inputTrigger);
			inputTrigger->ButtonChangeReceivers.ExecuteOnBound(i_VKey, bWentDown);
		}

		bool Init( )
		{
			GLib::SetKeyStateChangeCallback(ButtonChangeCallback);

			return true;
		}
	} // namespace Input
} // namespace Engine
