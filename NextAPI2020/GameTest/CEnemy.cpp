// Standard header
#include "stdafx.h"

// Internal includes
#include "CEnemy.h"

//--- Constructors and Destructor ---//
CEnemy::CEnemy()
{
	m_rewardAmount = 0.0f;
	m_damage = 0.0f;
	m_enemyType = EnemyType::Basic;
}

CEnemy::~CEnemy()
{
}



//--- Component Interface ---//
void CEnemy::Init()
{
}

void CEnemy::Update(float _deltaTime)
{
}



//--- Setters ---//
void CEnemy::SetRewardAmount(float _reward)
{
	m_rewardAmount = _reward;
}

void CEnemy::SetDamage(float _damage)
{
	m_damage = _damage;
}

void CEnemy::SetEnemyType(EnemyType _enemyType)
{
	m_enemyType = _enemyType;
}



//--- Getters ---//
float CEnemy::GetRewardAmount() const
{
	return m_rewardAmount;
}

float CEnemy::GetDamage() const
{
	return m_damage;
}

EnemyType CEnemy::GetEnemyType() const
{
	return m_enemyType;
}
