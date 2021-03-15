// Standard header
#include "stdafx.h"

// Internal includes
#include "SceneManager.h"



//--- Constructors and Destructor ---//
SceneManager::SceneManager()
{
	m_activeScene = nullptr;
}

SceneManager::~SceneManager()
{
	// Unload the active scene
	UnloadActiveScene();
}



//--- Methods ---//
void SceneManager::UnloadActiveScene()
{
	if (m_activeScene)
	{
		m_activeScene->Unload();

		// Clean up the scene memory
		delete m_activeScene;
		m_activeScene = nullptr;
	}
}

void SceneManager::UpdateActiveScene(float _deltaTime)
{
	m_activeScene->Update(_deltaTime);
}