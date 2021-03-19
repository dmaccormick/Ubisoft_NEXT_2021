#pragma once

enum class TileType
{
	Wall,
	EnemySpawn,
	EnemyPath,
	PlayerBase,
	TurretArea,
	TurretBuilt,
};

enum class MovementDir
{
	Up,
	Right,
	Down,
	Left,
};