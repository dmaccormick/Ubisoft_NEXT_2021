// Standard header
#include "stdafx.h"

// Standard includes
#include <fstream>

// Internal includes
#include "LevelLoader.h"
#include "Registry.h"
#include "CMovementTile.h"

//--- Constructors and Destructor ---//
LevelLoader::LevelLoader()
{
	m_tileMapping = {{'X', TileType::Wall},
					{'E', TileType::EnemySpawn}, 
					{'^', TileType::EnemyPath}, 
					{'>', TileType::EnemyPath}, 
					{'v', TileType::EnemyPath}, 
					{'<', TileType::EnemyPath}, 
					{'P', TileType::PlayerBase}, 
					{'o', TileType::TurretArea} };

	m_movementMapping = { {'^', MovementDir::Up}, 
						{'>', MovementDir::Right},
						{'v', MovementDir::Down},
						{'<', MovementDir::Left}};
}

LevelLoader::~LevelLoader()
{

}



//--- Methods ---//
bool LevelLoader::LoadLevel(std::string _filePath, Registry& _registry, std::vector<Entity*>& _levelPieces)
{
	// Read the file line by line
	std::ifstream file = std::ifstream(_filePath);
	std::vector<std::string> fileLines;
	std::string nextLine;

	if (file.good())
	{
		while (getline(file, nextLine))
			fileLines.push_back(nextLine);
	}
	else
		return false;

	file.close();

	// The first line should be the indicator for the starting movement direction
	char firstLineChar = fileLines[0].at(0);
	MovementDir startingMovementDir = m_movementMapping[firstLineChar];

	// The rest of the lines should dictate all of the different tiles in the level
	for (unsigned int row = 1; row < fileLines.size(); row++)
	{
		for (unsigned int col = 0; col < fileLines[row].size(); col++)
		{
			char tileChar = fileLines[row][col];
			TileType tileType = m_tileMapping[tileChar];

			auto tileEntity = _registry.CreateEntity("Tile");
			// TODO: attach the relevant components here
			// TODO: assign the relevant movement types here
			_levelPieces.push_back(tileEntity);
		}
	}

	return true;
}