// Standard header
#include "stdafx.h"

// Internal includes
#include "SceneManager.h"

//--- Statics ---//
SceneManager* SceneManager::m_instance = nullptr;



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
		// Clean up the scene memory
		delete m_activeScene;
		m_activeScene = nullptr;
	}
}

void SceneManager::UpdateActiveScene(float _deltaTime)
{
	m_activeScene->Update(_deltaTime);
}

void SceneManager::DrawActiveScene()
{
	m_activeScene->Draw();
}



//--- Getters ---//
SceneManager* SceneManager::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = new SceneManager();

	return m_instance;
}