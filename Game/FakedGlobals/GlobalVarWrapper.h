#pragma once
#include <vector>
#include "GameCommon\GameObject.h"
#include "GameCommon\PhysicsInfo.h"
#include "GameCommon\Renderable.h"

#include "Core\DataType\SmartPtr.h"

#include "EngineLow\Rendering\GLib\GLib.h"
#include "EngineLow\Rendering\GLib\BasicTypes.h"

#include "EngineHigh\CollisionSystem\AABB.h"

class BulletController;
class AsteroidController;

class GlobalVarWrapper {
public:
	GlobalVarWrapper(vector<SmartPtr<GameObject>> & i_AllActors,
		vector<PhysicsInfo> & i_AllPhysXInfos,
		vector<Renderable> & i_AllRenderables,
		vector<SmartPtr<BulletController>> & i_bulletControllers,
		GLib::Sprites::Sprite * i_bulletSprite,
		AABB i_bulletBox, vector<SmartPtr<AsteroidController>> & i_asteroidControllers,
		GLib::Sprites::Sprite * i_rockSmallSprite,
		AABB i_rockSmallBox, GLib::Sprites::Sprite * i_rockBigSprite, AABB i_rockBigBox
		);
//************************************************************************************
	vector<SmartPtr<GameObject>> & m_AllActors;
	vector<PhysicsInfo> & m_AllPhysXInfos;
	vector<Renderable> & m_AllRenderables;
	
	vector<SmartPtr<BulletController>> & m_bulletControllers;
	GLib::Sprites::Sprite * m_bulletSprite;
	AABB m_bulletBox;

	vector<SmartPtr<AsteroidController>> & m_asteroidControllers;
	GLib::Sprites::Sprite * m_rockSmallSprite;
	AABB m_rockSmallBox;

	GLib::Sprites::Sprite * m_rockBigSprite;
	AABB m_rockBigBox;
};