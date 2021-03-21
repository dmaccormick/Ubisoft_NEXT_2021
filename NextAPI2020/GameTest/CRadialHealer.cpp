// Standard header
#include "stdafx.h"

// Internal includes
#include "CRadialHealer.h"
#include "CHealth.h"
#include "EntityRadiusChecker.h"

//--- Constructors and Destructor ---//
CRadialHealer::CRadialHealer()
{
	m_radiusIndicator = nullptr;
	m_healRatePerSecond = 0.0f;
	m_targetTag = EntityTag::None;
	m_radius = 0.0f;
}

CRadialHealer::~CRadialHealer()
{
}



//--- Component Interface ---//
void CRadialHealer::Init()
{
	m_transform = GetComponent<CTransform>();
	m_radiusIndicator = GetComponent<CRadiusIndicator>();
}

void CRadialHealer::Update(float _deltaTime)
{
	float dtSeconds = _deltaTime / 1000.0f;

	m_radiusIndicator->SetRadius(m_radius);

	Vec2 center = m_transform->GetPosition();
	std::vector<CHealth*> healthCompsInRange = EntityRadiusChecker::GetTaggedComponentsInRange<CHealth>(m_registry, m_targetTag, center, m_radius);

	float healAmount = m_healRatePerSecond * dtSeconds;
	for (auto healthComp : healthCompsInRange)
		healthComp->Heal(healAmount);
}



//--- Setters ---//
void CRadialHealer::SetHealRatePerSecond(float _healRatePerSecond)
{
	m_healRatePerSecond = _healRatePerSecond;
}

void CRadialHealer::SetTargetEntityTag(EntityTag _targetTag)
{
	m_targetTag = _targetTag;
}

void CRadialHealer::SetRadius(float _radius)
{
	m_radius = _radius;
}


//--- Utility Functions ---//
bool CRadialHealer::IsEntityInRange(CHealth* _healthComp) const
{
	return false;
}