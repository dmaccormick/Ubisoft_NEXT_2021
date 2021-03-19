// Standard header
#include "stdafx.h"

// Internal includes
#include "SGame.h"
#include "LevelLoader.h"

//--- Statics ---//
std::string SGame::m_levelFolderPath = ".\\GameData\\Levels\\";



//--- Constructors and Destructor ---//
SGame::SGame()
{
	m_levelName = "Level_1.txt";
	m_levelPieces = std::vector<Entity*>();
}

SGame::~SGame()
{

}



//--- Scene Interface ---//
void SGame::Init()
{
	LoadLevel();
}

void SGame::Update(float _deltaTime)
{

}

void SGame::Draw()
{

}



//--- Game Logic Methods ---//
void SGame::LoadLevel()
{
	std::string fullFilePath = m_levelFolderPath + m_levelName;

	LevelLoader loader = LevelLoader();
	loader.LoadLevel(fullFilePath, m_registry, m_levelPieces);
}



//--- Setters ---//
void SGame::SetLevelName(std::string _levelName)
{
	m_levelName = _levelName;
}