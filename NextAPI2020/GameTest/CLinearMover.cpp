// Standard header
#include "stdafx.h"

// Internal includes
#include "CLinearMover.h"

//--- Constructors and Destructor ---//
CLinearMover::CLinearMover()
{
	m_transform = nullptr;
	m_movementDir = Vec2::Zero();
	m_movementSpeed = 0.0f;
	m_movementModifier = 1.0f;
}

CLinearMover::~CLinearMover()
{
	m_transform = nullptr;
}



//--- Component Interface ---//
void CLinearMover::Init()
{
	m_transform = GetComponent<CTransform>();
}

void CLinearMover::Update(float _deltaTime)
{
	float dtSeconds = _deltaTime / 1000.0f;
	
	// Move along the path. Scale the movement by delta time to keep it framerate independent
	Vec2 pos = m_transform->GetPosition();
	pos += (m_movementDir * m_movementSpeed * dtSeconds * m_movementModifier);
	m_transform->SetPosition(pos);
	
	// Clear any movement buffs for next frame
	m_movementModifier = 1.0f;
}



//--- Methods ---//
void CLinearMover::ApplyMovementModifier(float _modifierAmount)
{
	m_movementModifier += _modifierAmount;
}



//--- Setters ---//
void CLinearMover::SetMovementDirection(Vec2 _movementDir)
{
	m_movementDir = Vec2::Normalize(_movementDir);
}

void CLinearMover::SetMovementSpeed(float _movementSpeed)
{
	m_movementSpeed = _movementSpeed;
}



//--- Getters ---//
Vec2 CLinearMover::GetMovementDirection() const
{
	return m_movementDir;
}

float CLinearMover::GetMovementSpeed() const
{
	return m_movementSpeed;
}
