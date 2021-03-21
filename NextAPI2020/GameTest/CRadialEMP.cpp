// Standard header
#include "stdafx.h"

// Internal includes
#include "CRadialEMP.h"
#include "CRadialAimer.h"
#include "EntityRadiusChecker.h"

//--- Constructors and Destructor ---//
CRadialEMP::CRadialEMP()
{
	m_transform = nullptr;
	m_radiusIndicator = nullptr;
	m_targetTag = EntityTag::None;
	m_radius = 0.0f;
}

CRadialEMP::~CRadialEMP()
{
}



//--- Component Interface ---//
void CRadialEMP::Init()
{
	m_transform = GetComponent<CTransform>();
	m_radiusIndicator = GetComponent<CRadiusIndicator>();
}

void CRadialEMP::Update(float _deltaTime)
{
	m_radiusIndicator->SetRadius(m_radius);

	Vec2 center = m_transform->GetPosition();
	std::vector<CRadialAimer*> aimersInRange = EntityRadiusChecker::GetTaggedComponentsInRange<CRadialAimer>(m_registry, m_targetTag, center, m_radius);

	for (auto aimerComp : aimersInRange)
		aimerComp->Deactivate();
}

void CRadialEMP::SetTargetEntityTag(EntityTag _targetTag)
{
	m_targetTag = _targetTag;
}

void CRadialEMP::SetRadius(float _radius)
{
	m_radius = _radius;
}
