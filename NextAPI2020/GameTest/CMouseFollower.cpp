// Standard header
#include "stdafx.h"

// Internal includes
#include "CMouseFollower.h"
#include "App/app.h"

//--- Constructors and Destructor ---//
CMouseFollower::CMouseFollower()
{
	m_transform = nullptr;
}

CMouseFollower::~CMouseFollower()
{
}



//--- Component Interface ---//
void CMouseFollower::Init()
{
	m_transform = GetComponent<CTransform>();
}

void CMouseFollower::Update(float _deltaTime)
{
	float mouseX, mouseY;
	App::GetMousePos(mouseX, mouseY);
	m_transform->SetPosition(Vec2(mouseX, mouseY));
}