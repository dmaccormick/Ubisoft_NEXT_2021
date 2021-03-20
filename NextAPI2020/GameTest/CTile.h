#pragma once

// Internal includes
#include "Component.h"
#include "Vec2.h"

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

class CTile : public Component
{
public:
	//--- Constructors and Destructor ---//
	CTile();
	virtual ~CTile();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Setters ---//
	void SetTileType(TileType _tileType);
	void SetMovementDirection(Vec2 _direction);

	//--- Getters ---//
	TileType GetTileType() const;
	Vec2 GetMovementDirection() const;

private:
	//--- Private Data ---//
	TileType m_tileType;
	Vec2 m_movementDir;
};