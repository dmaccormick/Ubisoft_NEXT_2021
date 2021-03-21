// Standard header
#include "stdafx.h"

// Internal includes
#include "CRadialDamager.h"
#include "CHealth.h"
#include "EntityRadiusChecker.h"

//--- Constructors and Destructor ---//
CRadialDamager::CRadialDamager()
{
	m_transform = nullptr;
	m_radiusIndicator = nullptr;
	m_damageAmount = 0.0f;
	m_targetTag = EntityTag::None;
	m_radius = 0.0f;
}

CRadialDamager::~CRadialDamager()
{
}



//--- Component Interface ---//
void CRadialDamager::Init()
{
	m_transform = GetComponent<CTransform>();
	m_radiusIndicator = GetComponent<CRadiusIndicator>();
}

void CRadialDamager::Update(float _deltaTime)
{
	m_radiusIndicator->SetRadius(m_radius);
}



//--- Methods ---//
void CRadialDamager::TriggerDamage(Entity* _callingEntity)
{
	Vec2 center = m_transform->GetPosition();
	std::vector<CHealth*> healthCompsInRange = EntityRadiusChecker::GetTaggedComponentsInRange<CHealth>(m_registry, m_targetTag, center, m_radius);

	for (auto healthComp : healthCompsInRange)
		healthComp->Damage(m_damageAmount);
}



//--- Setters ---//
void CRadialDamager::SetDamageAmount(float _damageAmount)
{
	m_damageAmount = _damageAmount;
}

void CRadialDamager::SetTargetEntityTag(EntityTag _targetTag)
{
	m_targetTag = _targetTag;
}

void CRadialDamager::SetRadius(float _radius)
{
	m_radius = _radius;
}
