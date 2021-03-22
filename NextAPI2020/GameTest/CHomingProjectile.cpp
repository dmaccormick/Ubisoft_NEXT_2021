// Standard header
#include "stdafx.h"

// Internal includes
#include "CHomingProjectile.h"
#include "CHealth.h"

//--- Constructors and Destructor ---//
CHomingProjectile::CHomingProjectile()
{
	m_transform = nullptr;
	m_linearMover = nullptr;
	m_target = nullptr;
	m_shouldHome = true;
	m_homingDelay = 0.15f;
	m_homingDelayFinished = false;
	m_timeSinceStartHoming = 0.0f;
}

CHomingProjectile::~CHomingProjectile()
{
}



//--- Component Interface ---//
void CHomingProjectile::Init()
{
	m_transform = GetComponent<CTransform>();
	m_linearMover = GetComponent<CLinearMover>();
}

void CHomingProjectile::Update(float _deltaTime)
{
	// Start homing after a bit of time to make a better looking visual effect
	if (!m_homingDelayFinished)
	{
		m_timeSinceStartHoming += (_deltaTime / 1000.0f);
		if (m_timeSinceStartHoming >= m_homingDelay)
			m_homingDelayFinished = true;
	}
	else if (m_shouldHome)
	{
		// Continually realign the movement trajectory to home in on the target
		Vec2 vecToTarget = m_target->GetPosition() - m_transform->GetPosition();
		m_linearMover->SetMovementDirection(vecToTarget);
	}
}



//--- Methods ---//
void CHomingProjectile::ClearTarget(Entity* target)
{
	m_shouldHome = false;
}



//--- Setters ---//
void CHomingProjectile::SetTarget(CTransform* _target)
{
	m_target = _target;
	
	// Need to know if the target is destroyed. If it is, this bullet can no longer home in on it
	m_target->GetComponent<CHealth>()->AddOnDestroyCallback(std::bind(&CHomingProjectile::ClearTarget, this, P_ARG::_1));
}
