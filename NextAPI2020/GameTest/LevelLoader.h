#pragma once

// Standard includes
#include <vector>
#include <string>
#include <map>

// Internal includes
#include "Registry.h"
#include "CTile.h"
#include "Vec2.h"

struct LevelInfo
{
	std::string m_levelDataPath = "";
	const char* m_levelTilesetPath = "";
	Vec2 m_topLeftLoc = Vec2::Zero();
	float m_tileSize = 0.0f;
};

struct TileInfo
{
	TileInfo() 
	{
		m_tileType = TileType::Wall;
		m_movementDir = Vec2::Zero();
		m_colliderDimenions = Vec2::Zero();
		m_entityTag = EntityTag::None;
	}

	TileInfo(TileType _tileType, Vec2 _movementDir, Vec2 _colliderDimensions, EntityTag _entityTag)
		:	m_tileType(_tileType),
			m_movementDir(_movementDir),
			m_colliderDimenions(_colliderDimensions),
			m_entityTag(_entityTag)
	{}

	TileType m_tileType;
	Vec2 m_movementDir;
	Vec2 m_colliderDimenions;
	EntityTag m_entityTag;
};

class LevelLoader
{
public:
	//--- Constructors and Destructor ---//
	LevelLoader();
	~LevelLoader();

	//--- Methods ---//
	bool LoadLevel(const LevelInfo& _levelInfo, Registry& _registry, std::vector<Entity*>& _levelPieces);

private:
	//--- Private Data ---//
	std::map<char, TileInfo> m_tileMapping;
};