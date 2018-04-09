//Yitong 4/8/2018
#include <iostream>
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <time.h>

#include "Character/SmartMonsterController.h"
#include "Character/PlayerController.h"
#include "Engine.h"

extern bool SmartPointer_UnitTest();
extern bool Matrix_UnitTest();
extern bool Collision_UnitTest();

#define PI 3.14159265
#define player 0
//#define monster 1

using namespace std;

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow) {
	//SmartPointer_UnitTest();
	//Matrix_UnitTest();
	//Collision_UnitTest();
	Engine::initHeap(10 * 1024 * 1024, 2048);
	Engine::init();

	// IMPORTANT: first we need to initialize GLib
	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "GLibTest", -1, 1280, 720);
	float deltaTime;
	if (bSuccess)
	{
		Messaging messages;
		Engine::InputHub::inputTrigger = &messages;
		// IMPORTANT (if we want keypress info from GLib): Set a callback for notification of key presses
		//GLib::SetKeyStateChangeCallback(TestKeyCallback);

		//initialize players and monsters 
		vector<SmartPtr<GameObject>> allActors(4);
		vector<PhysicsInfo> allPhysXInfos(4);
		vector<Renderable> allRenderables(4);
		vector<SmartPtr<SmartMonsterController>> monsterControllers;
		SmartPtr<PlayerController> p_playerController = nullptr;
		{
			clock_t t;
			t = clock();
			ActorCreationManager ACM(4);
			ACM.initACM();
			ACM.CreateActor("data\\Player.lua", allActors[player], allPhysXInfos[player], allRenderables[player]);
			for (int i = 0; i < 3; i++) {
				ACM.CreateActor("data\\Monster.lua", allActors[i+1], allPhysXInfos[i+1], allRenderables[i+1]);
			}
			ACM.shutDownACM();
			t = clock() - t;
			DEBUG_PRINT_INFO("Elapsed time for creating actors: %f", (float)t);

			allActors[player]->zRotationDegree = 0;
			p_playerController = new PlayerController(allActors[player], &messages);
			allActors[player]->m_pController = p_playerController;

			allActors[1]->zRotationDegree = 45;//bottom monster
			
			allActors[2]->zRotationDegree = -30;//right monster
			allActors[2]->position = Vector2D(400, 0);

			allActors[3]->zRotationDegree = 15;
			allActors[3]->position = Vector2D(0, 350);//top monster
			//allActors[4]->zRotationDegree = 15;
			//allActors[4]->position = Vector2D(-400, 0);//left monster
			for (int i = 0; i < 3; i++) {//initialize all monster controllers
				monsterControllers.push_back(new SmartMonsterController(allActors[i+1], allActors[player]));
				allActors[i + 1]->m_pController = monsterControllers[i];
			}
		}
	
		bool bQuit = false;

		do
		{
			//DEBUG_PRINT_INFO("%f", Timing::CalcLastFrameTime_ms());
			deltaTime = Timing::CalcLastFrameTime_ms()/1000;
			// IMPORTANT: We need to let GLib do it's thing. 		
			GLib::Service(bQuit);
			
			//******************read input***************************************************************
			allPhysXInfos[player].forcedApplied = p_playerController->forceApplied;
			//********************run AI*****************************************************************
			for (int i = 0; i < 3; i++) {
				SmartPtr<GameObject> pGameObject = allPhysXInfos[i+1].m_pObject.Aquire();
				allPhysXInfos[i+1].forcedApplied = pGameObject->m_pController->updateGameObject();
			}
			//allActors[monster]->zRotationDegree = allActors[monster]->zRotationDegree + 45 * deltaTime;
			//********************run phyiscs****************************************************************
			//Physics::updateFrameVelocity(allPhysXInfos);
			Physics::updateAllObjects(allPhysXInfos, deltaTime);
			//DEBUG_PRINT_INFO("x = %f, y = %f", allPhysXInfos[2].velocity.x, allPhysXInfos[2].velocity.y);
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

		for (int i = 0; i < 4; i++) {
			if (allRenderables[i].m_pSprite) {
				GLib::Sprites::Release(allRenderables[i].m_pSprite);
			}
		}
		// IMPORTANT:  Tell GLib to shutdown, releasing resources.
		GLib::Shutdown();

		allActors.clear();
		allPhysXInfos.clear();
		allRenderables.clear();
		monsterControllers.clear();
	}

	Engine::shutDownHeap();

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
}