#include "GlobalVarWrapper.h"

GlobalVarWrapper::GlobalVarWrapper(vector<SmartPtr<GameObject>> & i_AllActors,
	vector<PhysicsInfo> & i_AllPhysXInfos,
	vector<Renderable> & i_AllRenderables,
	vector<SmartPtr<BulletController>> & i_bulletControllers,
	GLib::Sprites::Sprite * i_bulletSprite,
	AABB i_bulletBox, vector<SmartPtr<AsteroidController>> & i_asteroidControllers,
	GLib::Sprites::Sprite * i_rockSmallSprite,
	AABB i_rockSmallBox, GLib::Sprites::Sprite * i_rockBigSprite, AABB i_rockBigBox):
	m_AllActors(i_AllActors),
	m_AllPhysXInfos(i_AllPhysXInfos),
	m_AllRenderables(i_AllRenderables),
	m_bulletControllers(i_bulletControllers),
	m_bulletSprite(i_bulletSprite),
	m_bulletBox(i_bulletBox),
	m_asteroidControllers(i_asteroidControllers),
	m_rockSmallSprite(i_rockSmallSprite),
	m_rockSmallBox(i_rockSmallBox),
	m_rockBigSprite(i_rockBigSprite),
	m_rockBigBox(i_rockBigBox)
{

}