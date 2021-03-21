#pragma once

// Internal Includes
#include "Scene.h"

class SceneManager
{
protected:
	//--- Protected Constructor for Singleton ---//
	SceneManager();

public:
	//--- Destructor ---//
	~SceneManager();

	//--- Methods ---//
	template<typename SceneType> void LoadScene()
	{
		UnloadActiveScene();
		
		m_activeScene = new SceneType();
		m_activeScene->Init();
	}
	void UpdateActiveScene(float _deltaTime);
	void DrawActiveScene();
	void UnloadActiveScene();

	//--- Getters ---//
	static SceneManager* GetInstance();

private:
	//--- Private Data ---//
	static SceneManager* m_instance;
	Scene* m_activeScene;
};