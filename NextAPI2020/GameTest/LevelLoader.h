#pragma once

// Standard includes
#include <vector>
#include <string>
#include <map>

// Internal includes
#include "Registry.h"
#include "CMovementTile.h"

class LevelLoader
{
public:
	//--- Constructors and Destructor ---//
	LevelLoader();
	~LevelLoader();

	//--- Methods ---//
	bool LoadLevel(std::string _filePath, Registry& _registry, std::vector<Entity*>& _levelPieces);

private:
	//--- Private Data ---//
	std::map<char, TileType> m_tileMapping;
	std::map<char, MovementDir> m_movementMapping;
};