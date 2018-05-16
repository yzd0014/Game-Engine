//Yitong 5/4/2018
#include <iostream>
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <time.h>

#include "Character\AsteroidController.h"
#include "Character\PlayerController.h"
#include "Character\BulletController.h"
#include "FakedGlobals\GlobalVarWrapper.h"
#include "Engine.h"

extern bool SmartPointer_UnitTest();
extern bool Matrix_UnitTest();
extern bool Collision_UnitTest();
extern bool Matrix_StressUnitTest();

#define PI 3.14159265
#define player 0
#define numOfinitialActors 9

using namespace std;
void gameObjectDeletion(GlobalVarWrapper &i_globals);
void createAsteroids(GlobalVarWrapper &o_globals, Vector2D i_spawnLoc);

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow) {
	srand((unsigned int)time(NULL));
	//SmartPointer_UnitTest();
	//Matrix_UnitTest();
	//Collision_UnitTest();
	Matrix_StressUnitTest();
	PROFILING_PRINT_INFO("maxtrix stress test: %f ticks(%f ms)", g_Profiler.m_Allccumulators[matrix_StressUnitTest]->average(), g_Profiler.m_Allccumulators[matrix_StressUnitTest]->getAverageTime());
	
	Engine::initHeap(10 * 1024 * 1024, 2048);
	Engine::init();

	// IMPORTANT: first we need to initialize GLib
	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "GLibTest", -1, 1280, 720);
	if (bSuccess)
	{
		Messaging messages;
		Engine::InputHub::inputTrigger = &messages;
		//load all sprites and set bounding boxes
		AABB shipBox;
		GLib::Sprites::Sprite * shipSprite = Engine::CreateSprite("data\\Assets\\Ship.dds", shipBox);
		AABB rockBigBox;
		GLib::Sprites::Sprite * rockBigSprite = Engine::CreateSprite("data\\Assets\\RockBig.dds", rockBigBox);
		AABB rockSmallBox;
		GLib::Sprites::Sprite * rockSmallSprite = Engine::CreateSprite("data\\Assets\\RockSmall.dds", rockSmallBox);
		AABB bulletBox;
		GLib::Sprites::Sprite * bulletSprtie = Engine::CreateSprite("data\\Assets\\Bullet.dds", bulletBox);
		
		Vector2D spawnLoc[4];
		spawnLoc[0] = Vector2D(-640, 360);
		spawnLoc[1] = Vector2D(640, 360);
		spawnLoc[2] = Vector2D(640, -360);
		spawnLoc[3] = Vector2D(-640, -360);
		
		//initialize players and monsters 
		vector<SmartPtr<GameObject>> allActors(numOfinitialActors);
		vector<PhysicsInfo> allPhysXInfos(numOfinitialActors);
		vector<Renderable> allRenderables(numOfinitialActors);
		
		SmartPtr<PlayerController> p_playerController = nullptr;
		vector<SmartPtr<BulletController>> bulletControllers;
		vector<SmartPtr<AsteroidController>> asteroidControllers;
		
		//store all variables into a wrapper
		GlobalVarWrapper fakedGlobals(allActors, allPhysXInfos, allRenderables, 
			bulletControllers, bulletSprtie, bulletBox, asteroidControllers, rockSmallSprite, rockSmallBox, rockBigSprite, rockBigBox);
		{
			clock_t t;
			t = clock();
			ActorCreationManager ACM(numOfinitialActors);
			ACM.initACM();
			//initialize player
			ACM.CreateActor("data\\Ship.lua", allActors[player], allPhysXInfos[player], shipBox, allRenderables[player], shipSprite);
			//initialize all big asteroids
			ACM.CreateActor("data\\RockBig_0.lua", allActors[1], allPhysXInfos[1], rockBigBox, allRenderables[1], rockBigSprite);
			ACM.CreateActor("data\\RockBig_1.lua", allActors[2], allPhysXInfos[2], rockBigBox, allRenderables[2], rockBigSprite);
			ACM.CreateActor("data\\RockBig_2.lua", allActors[3], allPhysXInfos[3], rockBigBox, allRenderables[3], rockBigSprite);
			ACM.CreateActor("data\\RockBig_3.lua", allActors[4], allPhysXInfos[4], rockBigBox, allRenderables[4], rockBigSprite);
			ACM.CreateActor("data\\RockBig_4.lua", allActors[5], allPhysXInfos[5], rockBigBox, allRenderables[5], rockBigSprite);
			ACM.CreateActor("data\\RockBig_5.lua", allActors[6], allPhysXInfos[6], rockBigBox, allRenderables[6], rockBigSprite);
			ACM.CreateActor("data\\RockBig_6.lua", allActors[7], allPhysXInfos[7], rockBigBox, allRenderables[7], rockBigSprite);
			ACM.CreateActor("data\\RockBig_7.lua", allActors[8], allPhysXInfos[8], rockBigBox, allRenderables[8], rockBigSprite);
			ACM.shutDownACM();
			t = clock() - t;
			DEBUG_PRINT_INFO("Elapsed time for creating actors: %f ms", (float)t);
			//**************************************************************************************************
			//initialize player controller
			p_playerController = new PlayerController(allActors[player], &messages, fakedGlobals);
			allActors[player]->m_pController = p_playerController;
			//initialize all asteroid controllers
			int RockBigCounter = 0;
			for (int i = 1; i < numOfinitialActors; i++) {
				asteroidControllers.push_back(new AsteroidController(allActors[i]));
				asteroidControllers[RockBigCounter]->gameObjectIndex = i;
				asteroidControllers[RockBigCounter]->phyXInfoIndex = i;
				asteroidControllers[RockBigCounter]->renderableIndex = i;
				allActors[i]->m_pController = asteroidControllers[RockBigCounter];
				RockBigCounter++;
			}
		}
		bool bQuit = false;
		bool spawnGate = false;
		int locPicker = 0;
		//float respawnInterval;
		//game loop starts here
		float deltaTime;
		do
		{
			deltaTime = Timing::CalcLastFrameTime_ms()/1000;
			Engine::deltaTime = deltaTime;
			// IMPORTANT: We need to let GLib do it's thing. 		
			GLib::Service(bQuit);
			//********************run AI*****************************************************************
			p_playerController->updateGameObject();

			size_t asteroidsCounter = asteroidControllers.size();
			for (size_t i = 0; i < asteroidsCounter; i++) {
				asteroidControllers[i]->updateGameObject();
			}
			
			size_t bulletCounter = bulletControllers.size();
			for (size_t i = 0; i < bulletCounter; i++) {
				bulletControllers[i]->updateGameObject();
			}
			
			//spawn new asteroids
			if (asteroidControllers.size() < 8) spawnGate = true;
			else if (asteroidControllers.size() > 12) spawnGate = false;
			if (spawnGate) {
				createAsteroids(fakedGlobals, spawnLoc[locPicker]);
				locPicker++;
				if (locPicker == 4)locPicker = 0;
			}
			gameObjectDeletion(fakedGlobals);
			//********************run phyiscs****************************************************************
			Physics::updateAllObjects(allPhysXInfos, deltaTime);
			gameObjectDeletion(fakedGlobals);
			//********************render**********************************************************************
			bool overlapFlag = false;
			if (!bQuit)
			{
				// IMPORTANT: Tell GLib that we want to start rendering
				GLib::BeginRendering(overlapFlag);
				// Tell GLib that we want to render some sprites
				GLib::Sprites::BeginRendering();
				
				size_t numOfObjects = allRenderables.size();
				for (size_t i = 0; i < numOfObjects; i++) {
					if (allRenderables[i].m_pSprite) {
						SmartPtr<GameObject> temp_pObject = allRenderables[i].m_pObject.Aquire();
						GLib::Sprites::RenderSprite(*allRenderables[i].m_pSprite, { (float)temp_pObject->position.x, (float)temp_pObject->position.y }, (float)(temp_pObject->zRotationDegree / 180 * PI));
					}
				}
				// Tell GLib we're done rendering sprites
				GLib::Sprites::EndRendering();
				// IMPORTANT: Tell GLib we're done rendering
				GLib::EndRendering();
			}
			HeapManager::heapManager->collect();
		} while (bQuit == false);
		
		GLib::Sprites::Release(shipSprite);
		GLib::Sprites::Release(rockBigSprite);
		GLib::Sprites::Release(rockSmallSprite);
		GLib::Sprites::Release(bulletSprtie);
		// IMPORTANT:  Tell GLib to shutdown, releasing resources.
		GLib::Shutdown();
		PROFILING_PRINT_INFO("Average collision detection(%d times): %f ticks(%f ms)", 
			g_Profiler.m_Allccumulators[collisionDetection_foo]->m_Count,
			g_Profiler.m_Allccumulators[collisionDetection_foo]->average(), 
			g_Profiler.m_Allccumulators[collisionDetection_foo]->getAverageTime());
	}
	Engine::shutDownHeap();
#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
	return 0;
}

void gameObjectDeletion(GlobalVarWrapper &o_globals) {
	int bulletCounter = 0;
	while (bulletCounter < (int)o_globals.m_bulletControllers.size()) {
		SmartPtr<BulletController> pBulletController = o_globals.m_bulletControllers[bulletCounter];
		SmartPtr<GameObject> temp_gameObject = pBulletController->getGameObject().Aquire();
		if (temp_gameObject->active == false) {
			//delete current element from actor array
			//update last element's index
			if (pBulletController->gameObjectIndex != o_globals.m_AllActors.size() - 1) {
				o_globals.m_AllActors.back()->m_pController->gameObjectIndex = pBulletController->gameObjectIndex;
				o_globals.m_AllActors[pBulletController->gameObjectIndex] = o_globals.m_AllActors.back();
			}
			o_globals.m_AllActors.pop_back();
			o_globals.m_AllActors.shrink_to_fit();

			//delete current element from phyiscsInfor array
			//update last element's index
			if (pBulletController->phyXInfoIndex != o_globals.m_AllPhysXInfos.size() - 1) {
				o_globals.m_AllPhysXInfos.back().m_pObject.Aquire()->m_pController->phyXInfoIndex = pBulletController->phyXInfoIndex;
				o_globals.m_AllPhysXInfos[pBulletController->phyXInfoIndex] = o_globals.m_AllPhysXInfos.back();
			}
			o_globals.m_AllPhysXInfos.pop_back();
			o_globals.m_AllPhysXInfos.shrink_to_fit();

			//delete current element from renderable array
			if (pBulletController->renderableIndex != o_globals.m_AllRenderables.size() - 1) {
				o_globals.m_AllRenderables.back().m_pObject.Aquire()->m_pController->renderableIndex = pBulletController->renderableIndex;
				o_globals.m_AllRenderables[pBulletController->renderableIndex] = o_globals.m_AllRenderables.back();
			}
			o_globals.m_AllRenderables.pop_back();
			o_globals.m_AllRenderables.shrink_to_fit();

			//delete current controller
			if (bulletCounter != o_globals.m_bulletControllers.size() - 1) {
				o_globals.m_bulletControllers[bulletCounter] = o_globals.m_bulletControllers.back();
			}
			o_globals.m_bulletControllers.pop_back();
			o_globals.m_bulletControllers.shrink_to_fit();

			bulletCounter--;
		}
		bulletCounter++;
	}
	//************************************************************************************************************	
	int asteroidsCounter = 0;
	while (asteroidsCounter < (int)o_globals.m_asteroidControllers.size()) {
		SmartPtr<AsteroidController> pAsteroidController = o_globals.m_asteroidControllers[asteroidsCounter];
		SmartPtr<GameObject> pAsteroid = pAsteroidController->getGameObject().Aquire();
		if (pAsteroid->active == false) {
			//delete gameObject
			if (pAsteroidController->gameObjectIndex != o_globals.m_AllActors.size()-1) {
				o_globals.m_AllActors.back()->m_pController->gameObjectIndex = pAsteroidController->gameObjectIndex;
				o_globals.m_AllActors[pAsteroidController->gameObjectIndex] = o_globals.m_AllActors.back();
			}
			o_globals.m_AllActors.pop_back();
			o_globals.m_AllActors.shrink_to_fit();

			//delete phyiscsInfo
			if (pAsteroidController->phyXInfoIndex != o_globals.m_AllPhysXInfos.size() - 1) {
				o_globals.m_AllPhysXInfos.back().m_pObject.Aquire()->m_pController->phyXInfoIndex = pAsteroidController->phyXInfoIndex;
				o_globals.m_AllPhysXInfos[pAsteroidController->phyXInfoIndex] = o_globals.m_AllPhysXInfos.back();
			}
			o_globals.m_AllPhysXInfos.pop_back();
			o_globals.m_AllPhysXInfos.shrink_to_fit();

			//delete renderable
			if (pAsteroidController->renderableIndex != o_globals.m_AllRenderables.size() - 1) {
				o_globals.m_AllRenderables.back().m_pObject.Aquire()->m_pController->renderableIndex = pAsteroidController->renderableIndex;
				o_globals.m_AllRenderables[pAsteroidController->renderableIndex] = o_globals.m_AllRenderables.back();
			}
			o_globals.m_AllRenderables.pop_back();
			o_globals.m_AllRenderables.shrink_to_fit();
			
			//delete current controller
			if (asteroidsCounter != o_globals.m_asteroidControllers.size() - 1) {
				o_globals.m_asteroidControllers[asteroidsCounter] = o_globals.m_asteroidControllers.back();
			}
			o_globals.m_asteroidControllers.pop_back();
			o_globals.m_asteroidControllers.shrink_to_fit();

			asteroidsCounter--;
		}
		asteroidsCounter++;
	}
}

void createAsteroids(GlobalVarWrapper &o_globals, Vector2D i_spawnLoc) {
	//gameObject creation
	SmartPtr<GameObject> pRock = new GameObject("RockBig", i_spawnLoc, 100);
	pRock->zRotationDegree = (float)(rand() % 360);
	o_globals.m_AllActors.push_back(pRock);

	//phyiscsInfo creation
	o_globals.m_AllPhysXInfos.push_back(PhysicsInfo(pRock, 1.0f, 0.0001f));
	o_globals.m_AllPhysXInfos.back().m_BoundingBox = o_globals.m_rockBigBox;
	float randomAngle = (float)(rand() % 360);
	float vel_x = (float)(130 * cos(randomAngle / 180 * PI));
	float vel_y = (float)(130 * sin(randomAngle / 180 * PI));
	o_globals.m_AllPhysXInfos.back().setVelocity(Vector2D(vel_x, vel_y));

	//renderable creation
	Renderable rockRenderable;
	rockRenderable.m_pObject = pRock;
	rockRenderable.m_pSprite = o_globals.m_rockBigSprite;
	o_globals.m_AllRenderables.push_back(rockRenderable);

	//controller creation
	o_globals.m_asteroidControllers.push_back(new AsteroidController(pRock));
	o_globals.m_asteroidControllers.back()->gameObjectIndex = (int)o_globals.m_AllActors.size() - 1;
	o_globals.m_asteroidControllers.back()->phyXInfoIndex = (int)o_globals.m_AllPhysXInfos.size() - 1;
	o_globals.m_asteroidControllers.back()->renderableIndex = (int)o_globals.m_AllRenderables.size() - 1;
	pRock->m_pController = o_globals.m_asteroidControllers.back();
}

