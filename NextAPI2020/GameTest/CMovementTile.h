#pragma once

enum class TileType
{
	Wall,
	EnemySpawn,
	EnemyPath,
	PlayerBase,
	TurretArea,
	TurretBuilt,

	Count
};

enum class MovementDir
{
	Up,
	Right,
	Down,
	Left,
	None
};