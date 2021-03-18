// Standard header
#include "stdafx.h"

// Standard includes
#include <algorithm>
#include <vector>

// Internal includes
#include "STest.h"
#include "CTransform.h"
#include "CSprite.h"
#include "App/app.h"

//--- Constructors and Destructor ---//
STest::STest()
{
	m_player = NULL;
}

STest::~STest()
{
}



//--- Scene Interface ---//
void STest::Init()
{
	// Create the player entity
	{
		m_player = m_registry.CreateEntity("Player");

		CTransform* transformComp = m_registry.AddComponent<CTransform>(m_player);
		transformComp->SetPosition(400.0f, 400.0f);
		transformComp->SetScale(2.0f);

		CSprite* spriteComp = m_registry.AddComponent<CSprite>(m_player);
		spriteComp->LoadSprite(".\\TestData\\Test.bmp", 8, 4);
		spriteComp->SetRenderLayer(-1.0f);

		float animSpeed = 1.0f / 15.0f;
		spriteComp->CreateAnimation(0, animSpeed, { 0,1,2,3,4,5,6,7 }); // Backwards
		spriteComp->CreateAnimation(1, animSpeed, { 8,9,10,11,12,13,14,15 }); // Left
		spriteComp->CreateAnimation(2, animSpeed, { 16,17,18,19,20,21,22,23 }); // Right
		spriteComp->CreateAnimation(3, animSpeed, { 24,25,26,27,28,29,30,31 }); // Forwards
	}

	// Create the ship entity
	{
		auto entity = m_registry.CreateEntity("Ship");

		auto transformComp = m_registry.AddComponent<CTransform>(entity);
		transformComp->SetPosition(400.0f, 400.0f);

		auto spriteComp = m_registry.AddComponent<CSprite>(entity);
		spriteComp->LoadSprite(".\\TestData\\Ships.bmp", 2, 12);
		spriteComp->SetRenderLayer(-3.0f);

		spriteComp->SetFrame(2);
	}

	// Init all of the components in the registry
	m_registry.InitAll();
}

void STest::Update(float _deltaTime)
{
	auto playerTransform = m_registry.GetComponent<CTransform>(m_player);
	auto playerSprite = m_registry.GetComponent<CSprite>(m_player);

	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		playerSprite->SetAnimation(2);
		playerTransform->SetPosition(playerTransform->GetPosition() + Vec2::Right());
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		playerSprite->SetAnimation(1);
		playerTransform->SetPosition(playerTransform->GetPosition() + Vec2::Left());
	}

	// Update all of the components in the registry
	m_registry.UpdateAll(_deltaTime);
}

void STest::Draw()
{
	std::vector<CSprite*> sprites = m_registry.GetAllComponents<CSprite>();
	std::sort(sprites.begin(), sprites.end(), [](CSprite* _a, CSprite* _b) {return _a->GetRenderLayer() > _b->GetRenderLayer(); });

	for (auto sprite : sprites)
		sprite->DrawSprite();
}