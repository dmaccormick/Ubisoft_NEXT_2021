// Standard header
#include "stdafx.h"

// Internal includes
#include "CTransform.h"

//--- Constructors and Destructor ---//
CTransform::CTransform()
{
	m_pos = Vec2();
	m_rot = 0.0f;
	m_scale = 1.0f;
}

CTransform::~CTransform()
{
}



//--- Component Interface ---//
void CTransform::Init()
{
}

void CTransform::Update(float _deltaTime)
{
}



//--- Setters ---//
void CTransform::SetPosition(Vec2 _newPos)
{
	m_pos = _newPos;
}

void CTransform::SetPosition(float _newX, float _newY)
{
	m_pos = Vec2(_newX, _newY);
}

void CTransform::SetRotation(float _newRot)
{
	m_rot = _newRot;
}

void CTransform::SetScale(float _newScale)
{
	m_scale = _newScale;
}



//--- Getters ---//
Vec2 CTransform::GetPosition() const
{
	return m_pos;
}

float CTransform::GetRotation() const
{
	return m_rot;
}

float CTransform::GetScale() const
{
	return m_scale;
}
