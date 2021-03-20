// Standard header
#include "stdafx.h"

// Internal includes
#include "CTile.h"

//--- Constructors and Destructor ---//
CTile::CTile()
{
	m_tileType = TileType::Wall;
	m_movementDir = Vec2::Zero();
}

CTile::~CTile()
{

}



//--- Component Interface ---//
void CTile::Init()
{

}

void CTile::Update(float _deltaTime)
{

}



//--- Setters ---//
void CTile::SetTileType(TileType _tileType)
{
	m_tileType = _tileType;
}

void CTile::SetMovementDirection(Vec2 _movementDir)
{
	m_movementDir = _movementDir;
}



//--- Getters ---//
TileType CTile::GetTileType() const
{
	return m_tileType;
}

Vec2 CTile::GetMovementDirection() const
{
	return m_movementDir;
}