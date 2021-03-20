// Standard header
#include "stdafx.h"

// Internal includes
#include "CProjectile.h"

//--- Constructor and Destructor ---//
CProjectile::CProjectile()
{
	m_damage = 0.0f;
}

CProjectile::~CProjectile()
{
}



//--- Component Interface ---//
void CProjectile::Init()
{
}

void CProjectile::Update(float _deltaTime)
{
}



//--- Setters ---//
void CProjectile::SetDamage(float _damage)
{
	m_damage = _damage;
}



//--- Getters ---//
float CProjectile::GetDamage() const
{
	return m_damage;
}
