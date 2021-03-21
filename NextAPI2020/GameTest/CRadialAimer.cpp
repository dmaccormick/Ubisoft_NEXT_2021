// Standard header
#include "stdafx.h"

// Standard includes
#include <algorithm>
#include <math.h>

// Internal includes
#include "CRadialAimer.h"
#include "EntityRadiusChecker.h"

//--- Constructors and Destructor ---//
CRadialAimer::CRadialAimer()
{
	m_transform = nullptr;
	m_targetTag = EntityTag::None;
	m_currentTarget = nullptr;
	m_radiusIndicator = nullptr;
	m_radius = 0.0f;
	m_deactivated = false;
}

CRadialAimer::~CRadialAimer()
{
}



//--- Component Interface ---//
void CRadialAimer::Init()
{
	m_transform = GetComponent<CTransform>();
	m_radiusIndicator = GetComponent<CRadiusIndicator>();
}

void CRadialAimer::Update(float _deltaTime)
{
	// Normally, look for and aim at targets
	// If deactivated, can no longer aim. Also, spin wildly to provide feedback
	if (!m_deactivated)
	{
		m_radiusIndicator->SetRadius(m_radius);
		m_radiusIndicator->SetColor(Color(m_radiusIndicator->GetColor(), 0.2f));

		SelectNewTarget();
		FaceTarget();
	}
	else
	{
		m_currentTarget = nullptr;
		m_radiusIndicator->SetColor(Color(m_radiusIndicator->GetColor(), 0.0f));
		m_transform->SetRotation(m_transform->GetRotation() + 0.2f);
		m_deactivated = false;
	}
}



//--- Methods ---//
void CRadialAimer::FaceTarget()
{
	if (m_currentTarget)
	{
		// Calculate the angle in radians needed to face the target
		Vec2 vecToTarget = m_currentTarget->GetPosition() - m_transform->GetPosition();
		float newAngle = atan2(vecToTarget.GetY(), vecToTarget.GetX());
		m_transform->SetRotation(newAngle);
	}
}

void CRadialAimer::SelectNewTarget()
{
	// Find the closest enemy and set it to be the target
	Vec2 center = m_transform->GetPosition();
	std::vector<CTransform*> targetsInRange = EntityRadiusChecker::GetTaggedComponentsInRange<CTransform>(m_registry, m_targetTag, center, m_radius);

	float closestDistance = 10000000000.0f;
	CTransform* closestTarget = nullptr;

	for (auto target : targetsInRange)
	{
		auto targetPos = target->GetPosition();
		Vec2 vecToTarget = targetPos - center;
		float squaredDistance = vecToTarget.GetMagnitudeSquared();

		if (squaredDistance < closestDistance)
		{
			closestDistance = squaredDistance;
			closestTarget = target;
		}
	}

	m_currentTarget = closestTarget;
}

void CRadialAimer::Deactivate()
{
	m_deactivated = true;
}



//--- Setters ---//
void CRadialAimer::SetRadius(float _range)
{
	m_radius = _range;
}

void CRadialAimer::SetTargetEntityTag(EntityTag _targetTag)
{
	m_targetTag = _targetTag;
}



//--- Getters ---//
CTransform* CRadialAimer::GetTarget() const
{
	return m_currentTarget;
}

float CRadialAimer::GetRadius() const
{
	return m_radius;
}