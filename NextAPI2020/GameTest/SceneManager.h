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
	template<typename T> void LoadScene()
	{
		UnloadActiveScene();
		
		m_activeScene = new T();
		m_activeScene->Load();
	}
	void UpdateActiveScene(float _deltaTime);
	void UnloadActiveScene();

private:
	//--- Data ---//
	Scene* m_activeScene;
};