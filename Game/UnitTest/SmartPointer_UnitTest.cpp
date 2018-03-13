#include <assert.h>
#include <vector>

#include "Engine.h"
class TestClass {
public:
	TestClass() :
		pObject(nullptr)
	{
	}
	TestClass(SmartPtr<GameObject> i_pObject) :
		pObject(i_pObject)
	{

	}
	void foo() {
		DEBUG_PRINT_INFO("Hello World!");
	}
	~TestClass() {
		pObject = nullptr;
	}
private:
	SmartPtr<GameObject> pObject;
};

bool SmartPointer_UnitTest() {
	Engine::initHeap(1024 * 1024, 2048);

	{
		//text normal constructor and assignment operator
		vector<SmartPtr<GameObject>> gameObjects;
		gameObjects.push_back(SmartPtr<GameObject>(new GameObject("Ronaldo", Vector2D(0, 0), 100)));
		{
			SmartPtr<GameObject> pObject_0 = gameObjects[0];
			pObject_0 = pObject_0;//test self assignment
			gameObjects[0] = pObject_0;//test self assignment
			
			//test weak pointer 
			WeakPtr<GameObject> pWeak = pObject_0;
			WeakPtr<GameObject> pWeak_2 = pWeak;
			SmartPtr<GameObject> pObject_new = pWeak.Aquire();
			
			WeakPtr<GameObject> pWeak_3(new GameObject("Ronaldo", Vector2D(0, 0), 90));
			pWeak_2 = pWeak_3;
			pWeak_2 = nullptr;
			
		}

	}
	{
		SmartPtr<GameObject> pObject_1(nullptr);//test null constructor
		SmartPtr<GameObject> pObject_2a(new GameObject("Ronaldo", Vector2D(0, 0), 90));
		SmartPtr<GameObject> pObject_2b = pObject_2a; //test copy constructor
		SmartPtr<GameObject> pObject_2c(new GameObject("Adriano", Vector2D(0, 0), 100));
		pObject_2c = pObject_2b;//test assignment operator
								//test comparison operator
		assert(pObject_1 == nullptr);
		assert(pObject_2a != pObject_1);
		assert(pObject_2a == pObject_2c);

	}

	{
		//test member function access operator
		SmartPtr<GameObject> pObject_3(new GameObject("Robinho", Vector2D(0, 0), 100));
		SmartPtr<TestClass> pObject_4(new TestClass(pObject_3));
		pObject_4->foo();
		(*pObject_4).foo();

	}


	Engine::shutDownHeap();
	return true;
}