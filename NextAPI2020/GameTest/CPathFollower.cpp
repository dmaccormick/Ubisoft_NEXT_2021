// Standard header
#include "stdafx.h"

// Internal includes
#include "CPathFollower.h"

//--- Constructors and Destructor ---//
CPathFollower::CPathFollower()
{
	m_transform = nullptr;
	m_movementDir = Vec2::Zero();
	m_movementSpeed = 0.0f;
}

CPathFollower::~CPathFollower()
{
	m_transform = nullptr;
}



//--- Component Interface ---//
void CPathFollower::Init()
{
	m_transform = m_entity->GetComponent<CTransform>();
}

void CPathFollower::Update(float _deltaTime)
{
	float dtSeconds = _deltaTime / 1000.0f;
	
	// Move along the path. Scale the movement by delta time to keep it framerate independent
	Vec2 pos = m_transform->GetPosition();
	pos += (m_movementDir * m_movementSpeed * dtSeconds);
	m_transform->SetPosition(pos);
}



//--- Setters ---//
void CPathFollower::SetMovementDirection(Vec2 _movementDir)
{
	m_movementDir = _movementDir;
}

void CPathFollower::SetMovementSpeed(float _movementSpeed)
{
	m_movementSpeed = _movementSpeed;
}



//--- Getters ---//
Vec2 CPathFollower::GetMovementDirection() const
{
	return m_movementDir;
}

float CPathFollower::GetMovementSpeed() const
{
	return m_movementSpeed;
}
