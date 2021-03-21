// Standard header
#include "stdafx.h"

// Standard includes
#include <fstream>

// Internal includes
#include "LevelLoader.h"
#include "Registry.h"
#include "CTile.h"
#include "CTransform.h"
#include "CSprite.h"
#include "CBoxCollider.h"

//--- Constructors and Destructor ---//
LevelLoader::LevelLoader()
{
	m_tileMapping =	{
		{'X',	TileInfo(TileType::Wall,		Vec2::Zero(),	Vec2::Zero(),		EntityTag::None)},
		{'E',	TileInfo(TileType::EnemySpawn,	Vec2::Zero(),	Vec2(32.0f, 32.0f),	EntityTag::EnemySpawn)},
		{'^',	TileInfo(TileType::EnemyPath,	Vec2::Up(),		Vec2(5.0f, 32.0f),	EntityTag::EnemyPath)},
		{'>',	TileInfo(TileType::EnemyPath,	Vec2::Right(),	Vec2(32.0f, 5.0f),	EntityTag::EnemyPath)},
		{'v',	TileInfo(TileType::EnemyPath,	Vec2::Down(),	Vec2(5.0f, 32.0f),	EntityTag::EnemyPath)},
		{'<',	TileInfo(TileType::EnemyPath,	Vec2::Left(),	Vec2(32.0f, 5.0f),	EntityTag::EnemyPath)},
		{'P',	TileInfo(TileType::PlayerBase,	Vec2::Zero(),	Vec2(32.0f, 32.0f),	EntityTag::PlayerBase)},
		{'o',	TileInfo(TileType::TurretArea,	Vec2::Zero(),	Vec2::Zero(),		EntityTag::TurretBuildArea)}
	};
}

LevelLoader::~LevelLoader()
{

}



//--- Methods ---//
bool LevelLoader::LoadLevel(const LevelInfo& _levelInfo, Registry& _registry, std::vector<Entity*>& _levelPieces)
{
	// Read the file line by line
	std::ifstream file = std::ifstream(_levelInfo.m_levelDataPath);
	std::string firstLine = "";
	std::vector<std::string> tileDescLines;
	std::string nextLine;

	if (file.good())
	{
		while (getline(file, nextLine))
		{
			if (firstLine == "")
				firstLine = nextLine;
			else
				tileDescLines.push_back(nextLine);
		}
	}
	else
		return false;

	file.close();

	// The first line should be the indicator for the starting movement direction
	// This one is special because the spawn indicator is noted by its own symbol and so cannot also have a direction arrow
	char firstLineChar = firstLine.at(0);
	Vec2 startingMovementDir = m_tileMapping[firstLineChar].m_movementDir;

	// The rest of the lines should dictate all of the different tiles in the level
	for (unsigned int row = 0; row < tileDescLines.size(); row++)
	{
		for (unsigned int col = 0; col < tileDescLines[row].size(); col++)
		{
			// Determine the type of tile
			char tileChar = tileDescLines[row][col];
			TileType tileType = m_tileMapping[tileChar].m_tileType;

			// Create the tile entity
			std::string tileEntityName = "Tile_R" + std::to_string(row) + "C" + std::to_string(col);
			auto tileEntity = _registry.CreateEntity(tileEntityName, m_tileMapping[tileChar].m_entityTag);

			// Add and configure the tile component
			auto tileComp = _registry.AddComponent<CTile>(tileEntity);
			tileComp->SetTileType(tileType);
			if (tileType == TileType::EnemySpawn)
				tileComp->SetMovementDirection(startingMovementDir);
			else
				tileComp->SetMovementDirection(m_tileMapping[tileChar].m_movementDir);
			
			// Add and configure the transform component
			auto transformComp = _registry.AddComponent<CTransform>(tileEntity);
			Vec2 tileOffset = Vec2((float)col * _levelInfo.m_tileSize, (float)row * -_levelInfo.m_tileSize);
			transformComp->SetPosition(_levelInfo.m_topLeftLoc + tileOffset);
			transformComp->SetScale(2.0f);

			// Add and configure the sprite component
			auto spriteComp = _registry.AddComponent<CSprite>(tileEntity);
			spriteComp->LoadSprite(_levelInfo.m_levelTilesetPath, (int)TileType::Count, 1);
			spriteComp->SetFrame((int)m_tileMapping[tileChar].m_tileType);
			spriteComp->SetRenderLayer(10.0f);

			// Optionally add a box collider component as well
			Vec2 colliderDimensions = m_tileMapping[tileChar].m_colliderDimenions;
			if (colliderDimensions != Vec2::Zero())
			//if (m_tileMapping[tileChar].m_tileType == TileType::EnemySpawn)
			{
				auto boxColliderComp = _registry.AddComponent<CBoxCollider>(tileEntity);
				boxColliderComp->SetBaseDimensions(colliderDimensions);
			}
			
			// Finish by putting the entity into the list to be sent back
			_levelPieces.push_back(tileEntity);
		}
	}

	return true;
}