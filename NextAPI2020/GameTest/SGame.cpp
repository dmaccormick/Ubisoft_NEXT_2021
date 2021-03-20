// Standard header
#include "stdafx.h"

// Internal includes
#include "SGame.h"
#include "LevelLoader.h"
#include "CSprite.h"

//--- Statics ---//
std::string SGame::m_assetFolderPath = ".\\GameData\\";



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

	m_registry.InitAll();
}

void SGame::Update(float _deltaTime)
{
	m_registry.UpdateAll(_deltaTime);
}

void SGame::Draw()
{
	// Sort the sprites by draw order and then render them all
	std::vector<CSprite*> sprites = m_registry.GetAllComponentsByType<CSprite>();
	std::sort(sprites.begin(), sprites.end(), [](CSprite* _a, CSprite* _b) {return _a->GetRenderLayer() > _b->GetRenderLayer(); });

	for (auto sprite : sprites)
		sprite->DrawSprite();
}



//--- Game Logic Methods ---//
void SGame::LoadLevel()
{
	LevelInfo levelInfo;
	levelInfo.m_topLeftLoc = Vec2(300.0f, 600.0f);
	levelInfo.m_levelDataPath = m_assetFolderPath + "Levels\\" + m_levelName;
	levelInfo.m_levelTilesetPath = m_assetFolderPath + "Sprites\\Level_Tiles_1.bmp";
	levelInfo.m_tileSize = 64.0f;

	LevelLoader loader = LevelLoader();
	loader.LoadLevel(levelInfo, m_registry, m_levelPieces);
}



//--- Setters ---//
void SGame::SetLevelName(std::string _levelName)
{
	m_levelName = _levelName;
}