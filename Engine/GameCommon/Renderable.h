#pragma once
#include "Core\DataType\WeakPtr.h"
#include "GameObject.h"
#include "EngineLow\Rendering\GLib\GLib.h"
#include "EngineLow\Rendering\GLib\BasicTypes.h"

struct Renderable {
	WeakPtr<GameObject> m_pObject;
	GLib::Sprites::Sprite * m_pSprite;
};