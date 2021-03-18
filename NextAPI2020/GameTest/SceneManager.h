#pragma once

// Internal Includes
#include "Scene.h"

class SceneManager
{
public:
	//--- Constructors and Destructor ---//
	SceneManager();
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

private:
	//--- Data ---//
	Scene* m_activeScene;
};