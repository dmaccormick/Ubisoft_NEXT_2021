// Standard header
#include "stdafx.h"

// Internal includes
#include "CRadialSpeeder.h"
#include "CLinearMover.h"
#include "EntityRadiusChecker.h"

//--- Constructors and Destructor ---//
CRadialSpeeder::CRadialSpeeder()
{
	m_transform = nullptr;
	m_radiusIndicator = nullptr;
	m_speedModifier = 0.0f;
	m_targetTag = EntityTag::None;
	m_radius = 0.0f;
}

CRadialSpeeder::~CRadialSpeeder()
{
}



//--- Component Interface ---//
void CRadialSpeeder::Init()
{
	m_transform = GetComponent<CTransform>();
	m_radiusIndicator = GetComponent<CRadiusIndicator>();
}

void CRadialSpeeder::Update(float _deltaTime)
{
	m_radiusIndicator->SetRadius(m_radius);

	Vec2 center = m_transform->GetPosition();
	std::vector<CLinearMover*> moveCompsInRange = EntityRadiusChecker::GetTaggedComponentsInRange<CLinearMover>(m_registry, m_targetTag, center, m_radius);

	for (auto moveComp : moveCompsInRange)
		moveComp->ApplyMovementModifier(m_speedModifier);
}



//--- Setters ---//
void CRadialSpeeder::SetSpeedModifier(float _modifier)
{
	m_speedModifier = _modifier;
}

void CRadialSpeeder::SetTargetEntityTag(EntityTag _targetTag)
{
	m_targetTag = _targetTag;
}

void CRadialSpeeder::SetRadius(float _radius)
{
	m_radius = _radius;
}
