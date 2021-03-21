// Standard header
#include "stdafx.h"

// Standard includes
#include <algorithm>
#include <math.h>

// Internal includes
#include "CTurretAimer.h"

//--- Statics ---//
const std::vector<Entity*>* CTurretAimer::m_enemies = nullptr;



//--- Constructors and Destructor ---//
CTurretAimer::CTurretAimer()
{
	m_transform = nullptr;
	m_enemies = nullptr;
	m_currentTarget = nullptr;
	m_radiusIndicator = nullptr;
	m_range = 0.0f;
}

CTurretAimer::~CTurretAimer()
{
}



//--- Component Interface ---//
void CTurretAimer::Init()
{
	m_transform = GetComponent<CTransform>();
	m_radiusIndicator = GetComponent<CRadiusIndicator>();
}

void CTurretAimer::Update(float _deltaTime)
{
	m_radiusIndicator->SetRadius(m_range);

	SelectNewTarget();
	FaceTarget();
}



//--- Methods ---//
void CTurretAimer::FaceTarget()
{
	if (m_currentTarget)
	{
		// Calculate the angle in radians needed to face the target
		Vec2 vecToTarget = m_currentTarget->GetPosition() - m_transform->GetPosition();
		float newAngle = atan2(vecToTarget.GetY(), vecToTarget.GetX());
		m_transform->SetRotation(newAngle);
	}
}

void CTurretAimer::SelectNewTarget()
{
	if (m_enemies)
	{
		// Cache some values to prevent extra calls and calculations
		float rangeSquared = m_range * m_range;
		Vec2 thisPos = m_transform->GetPosition();

		// Find the closest enemy and set it to be the target
		float closestDistance = 10000000000.0f;
		CTransform* closestTarget = nullptr;

		for (auto enemy : *m_enemies)
		{
			auto enemyTransform = enemy->GetComponent<CTransform>();
			auto enemyPos = enemyTransform->GetPosition();
			Vec2 vecToEnemy = enemyPos - thisPos;
			float squaredDistance = vecToEnemy.GetMagnitudeSquared();

			if (squaredDistance <= rangeSquared && squaredDistance < closestDistance)
			{
				closestDistance = squaredDistance;
				closestTarget = enemyTransform;
			}
		}

		m_currentTarget = closestTarget;
	}
}



//--- Setters ---//
void CTurretAimer::SetEnemyList(const std::vector<Entity*>& _enemies)
{
	m_enemies = &_enemies;
}

void CTurretAimer::SetRange(float _range)
{
	m_range = _range;
}



//--- Getters ---//
CTransform* CTurretAimer::GetTarget() const
{
	return m_currentTarget;
}

float CTurretAimer::GetRange() const
{
	return m_range;
}