// Standard header
#include "stdafx.h"

// Internal includes
#include "CBoxCollider.h"

//--- Constructors and Destructor ---//
CBoxCollider::CBoxCollider()
{
	m_transform = nullptr;
	m_dimensions = Vec2(1.0f, 1.0f);
	m_entityOffset = Vec2(0.0f, 0.0f);
	m_min = Vec2(-0.5f, -0.5f);
	m_max = Vec2(0.5f, 0.5f);
}

CBoxCollider::~CBoxCollider()
{
	m_transform = nullptr;
}



//--- Component Interface ---//
void CBoxCollider::Init()
{
	m_transform = m_entity->GetComponent<CTransform>();
}

void CBoxCollider::Update(float _deltaTime)
{
	const Vec2 newEntityPos = m_transform->GetPosition();

	RecalculateMinAndMax(newEntityPos);
}



//--- Setters ---//
void CBoxCollider::SetDimensions(float _width, float _height)
{
	SetDimensions(Vec2(_width, _height));
}

void CBoxCollider::SetDimensions(Vec2 _dimensions)
{
	m_dimensions = _dimensions;
}

void CBoxCollider::SetDimensions(CSimpleSprite* _sprite)
{
	SetDimensions(_sprite->GetWidth(), _sprite->GetHeight());
}

void CBoxCollider::SetEntityOffset(float _x, float _y)
{
	SetEntityOffset(Vec2(_x, _y));
}

void CBoxCollider::SetEntityOffset(Vec2 _offset)
{
	m_entityOffset = _offset;
}



//--- Getters ---//
Vec2 CBoxCollider::GetMin() const
{
	return m_min;
}

Vec2 CBoxCollider::GetMax() const
{
	return m_max;
}

void CBoxCollider::GetMinAndMax(Vec2& _min, Vec2& _max)
{
	_min = m_min;
	_max = m_max;
}



//--- Utility Methods ---//
void CBoxCollider::RecalculateMinAndMax(const Vec2& _entityPos)
{
	Vec2 offsetPos = _entityPos + m_entityOffset;
	Vec2 halfDimensions = m_dimensions * 0.5f;

	m_max = offsetPos + Vec2(halfDimensions.GetX(), halfDimensions.GetY());
	m_min = offsetPos - Vec2(halfDimensions.GetX(), halfDimensions.GetY());
}