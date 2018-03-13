//Yitong 3/4/2018
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

#define w 0
#define a 1
#define s 2
#define d 3

#define player 0
#define monster 1

using namespace std;
char globalUserInput = 0;

void TestKeyCallback(unsigned int i_VKeyID, bool bWentDown)
{
#ifdef _DEBUG
	const size_t	lenBuffer = 65;
	char			Buffer[lenBuffer];

	sprintf_s(Buffer, lenBuffer, "VKey 0x%04x went %s\n", i_VKeyID, bWentDown ? "down" : "up");
	OutputDebugStringA(Buffer);
#endif // __DEBUG
	if (i_VKeyID == 0x57 && globalUserInput == 0 && bWentDown) {
		globalUserInput = 'w';
		return;

	}
	if (i_VKeyID == 0x53 && globalUserInput == 0 && bWentDown) {
		globalUserInput = 's';
		return;
		
	}
	if (i_VKeyID == 0x41 && globalUserInput == 0 && bWentDown) {
		globalUserInput = 'a';
		return;
		
	}
	if (i_VKeyID == 0x44 && globalUserInput == 0 && bWentDown) {
		globalUserInput = 'd';
		return;
	}
	if (i_VKeyID == 0x57 && globalUserInput == 'w' && !bWentDown) {
		globalUserInput = 0;
		return;

	}
	if (i_VKeyID == 0x53 && globalUserInput == 's' && !bWentDown) {
		globalUserInput = 0;
		return;

	}
	if (i_VKeyID == 0x41 && globalUserInput == 'a' && !bWentDown) {
		globalUserInput = 0;
		return;

	}
	if (i_VKeyID == 0x44 && globalUserInput == 'd' && !bWentDown) {
		globalUserInput = 0;
		return;
	}
}

void Renderer(Renderable & i_renderable) {
	if (i_renderable.m_pSprite) {
		SmartPtr<GameObject> temp_pObject = i_renderable.m_pObject.Aquire();
		GLib::Sprites::RenderSprite(*i_renderable.m_pSprite, { (float)temp_pObject->getPosition().get_x(), (float)temp_pObject->getPosition().get_y() }, 0.0f);
	}
}

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow) {
	//SmartPointer_UnitTest();
	//Matrix_UnitTest();
	Collision_UnitTest();
	Engine::initHeap(10 * 1024 * 1024, 2048);

	// IMPORTANT: first we need to initialize GLib
	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "GLibTest", -1, 1280, 720);
	float deltaTime;
	if (bSuccess)
	{
		// IMPORTANT (if we want keypress info from GLib): Set a callback for notification of key presses
		GLib::SetKeyStateChangeCallback(TestKeyCallback);

		//initialize players and monsters 
		vector<SmartPtr<GameObject>> allActors(2);
		vector<PhysicsInfo> allPhysXInfos(2);
		vector<Renderable> allRenderables(2);
		
		{
			clock_t t;
			t = clock();
			ActorCreationManager ACM(2);
			ACM.initACM();
			ACM.CreateActor("data\\Player.lua", allActors[player], allPhysXInfos[player], allRenderables[player]);
			ACM.CreateActor("data\\Monster.lua", allActors[monster], allPhysXInfos[monster], allRenderables[monster]);
			ACM.shutDownACM();
			t = clock() - t;
			DEBUG_PRINT_INFO("Elapsed time for creating actors: %f", (float)t);
			AABB playerBB;
			playerBB.center = Vector2D(0, 82);
			playerBB.extends = Vector2D(44, 82);
			allPhysXInfos[player].m_BoundingBox = playerBB;

			AABB monsterBB;
			monsterBB.center = Vector2D(0, 118);
			monsterBB.extends = Vector2D(160, 118);
			allPhysXInfos[monster].m_BoundingBox = monsterBB;
		}
	
		bool bQuit = false;

		do
		{
			//DEBUG_PRINT_INFO("%f", Timing::CalcLastFrameTime_ms());
			deltaTime = Timing::CalcLastFrameTime_ms();
			// IMPORTANT: We need to let GLib do it's thing. 		
			GLib::Service(bQuit);
			
			//******************read input***************************************************************
			Vector2D force4Player;
			if (globalUserInput == 'w') {//up
				force4Player = Vector2D(0, 1);
			}
			else if (globalUserInput == 's') {//down
				force4Player = Vector2D(0, -1);
			}
			else if (globalUserInput == 'a') {//left
				force4Player = Vector2D(-1, 0);
			}
			else if (globalUserInput == 'd') {//right
				force4Player = Vector2D(1, 0);
			}
			else {
				force4Player = Vector2D(0, 0);
			}
			
			//********************run AI*****************************************************************
			Vector2D force4Monster;
			Vector2D dir = allActors[player]->getPosition() - allActors[monster]->getPosition();
			if (dir.get_x() != 0 || dir.get_y() != 0) {
				dir.normalize();
				//dir = dir * 0.01f;
				force4Monster = dir * 0.005f;
			}
			else {
				force4Monster = Vector2D(0.0f, 0.0f);
			}

			//********************run phyiscs****************************************************************
			Vector2D velocity_player = Physics::update(allPhysXInfos[player], force4Player, deltaTime);
			Vector2D velocity_monster = Physics::update(allPhysXInfos[monster], force4Monster, deltaTime);

			{
				Engine::CollisionDetection(allPhysXInfos[player], allPhysXInfos[monster], velocity_player, velocity_monster, deltaTime);
				SmartPtr<GameObject> player_Object = allPhysXInfos[player].getGameObject().Aquire();
				player_Object->position = player_Object->position + velocity_player * deltaTime;
				
				SmartPtr<GameObject> monster_Object = allPhysXInfos[monster].getGameObject().Aquire();
				monster_Object->position = monster_Object->position + velocity_monster * deltaTime;
			}
			

			//********************render**********************************************************************
			if (!bQuit)
			{
				// IMPORTANT: Tell GLib that we want to start rendering
				GLib::BeginRendering();
				// Tell GLib that we want to render some sprites
				GLib::Sprites::BeginRendering();
				
				for (int i = 0; i < 2; i++) {
					Renderer(allRenderables[i]);
				}
				// Tell GLib we're done rendering sprites
				GLib::Sprites::EndRendering();
				// IMPORTANT: Tell GLib we're done rendering
				GLib::EndRendering();
			}
			HeapManager::heapManager->collect();
		} while (bQuit == false);

		for (int i = 0; i < 2; i++) {
			if (allRenderables[i].m_pSprite) {
				GLib::Sprites::Release(allRenderables[i].m_pSprite);
			}
		}
		// IMPORTANT:  Tell GLib to shutdown, releasing resources.
		GLib::Shutdown();

		allActors.clear();
		allPhysXInfos.clear();
		allRenderables.clear();
	}

	Engine::shutDownHeap();

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
}




