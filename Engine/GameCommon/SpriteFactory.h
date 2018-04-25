#pragma once
#include "EngineLow\Rendering\GLib\GLib.h"
#include "EngineLow\Rendering\GLib\BasicTypes.h"
#include "EngineHigh\CollisionSystem\AABB.h"

namespace Engine {
	GLib::Sprites::Sprite * CreateSprite(const char * i_pFilename, AABB & o_BoundingBox);
}