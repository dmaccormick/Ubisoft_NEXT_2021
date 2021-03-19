// Standard header
#include "stdafx.h"

// Standard includes
#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>

// Internal includes
#include "STest.h"
#include "CTransform.h"
#include "CSprite.h"
#include "CBoxCollider.h"
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

		CBoxCollider* boxCollider = m_registry.AddComponent<CBoxCollider>(m_player);
		boxCollider->SetBaseDimensions(spriteComp->GetSprite());
		boxCollider->AddCollisionListener(std::bind(&STest::PrintCollision, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		boxCollider->AddCollisionListener(std::bind(&STest::SpinPlayer, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
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

		CBoxCollider* boxCollider = m_registry.AddComponent<CBoxCollider>(entity);
		boxCollider->SetBaseDimensions(spriteComp->GetSprite());
	}

	// Init all of the components in the registry
	m_registry.InitAll();
}

void STest::Update(float _deltaTime)
{
	// Grab the player info
	auto playerTransform = m_player->GetComponent<CTransform>();
	auto playerSprite = m_player->GetComponent<CSprite>();

	// Move the player around with the controller
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
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		playerSprite->SetAnimation(3);
		playerTransform->SetPosition(playerTransform->GetPosition() + Vec2::Up());
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		playerSprite->SetAnimation(0);
		playerTransform->SetPosition(playerTransform->GetPosition() + Vec2::Down());
	}

	// Update all of the components in the registry
	m_registry.UpdateAll(_deltaTime);

	// Grab all of the colliders and check for collisions between them
	std::vector<CBoxCollider*> colliders = m_registry.GetAllComponentsByType<CBoxCollider>();
	if (colliders.size() > 1)
	{
		for (auto collider = colliders.begin(); collider < colliders.end() - 1; collider++)
		{
			Vec2 overlap = Vec2();
			auto collider1 = *collider;
			auto collider2 = *(collider + 1);

			if (CBoxCollider::CheckCollision(collider1, collider2, overlap))
			{
				collider1->OnCollision(collider2, overlap);
				collider2->OnCollision(collider1, overlap);
			}
		}
	}
}

void STest::Draw()
{
	std::vector<CSprite*> sprites = m_registry.GetAllComponentsByType<CSprite>();
	std::sort(sprites.begin(), sprites.end(), [](CSprite* _a, CSprite* _b) {return _a->GetRenderLayer() > _b->GetRenderLayer(); });

	for (auto sprite : sprites)
		sprite->DrawSprite();

	// Grab all of the colliders and check for collisions between them
	std::vector<CBoxCollider*> colliders = m_registry.GetAllComponentsByType<CBoxCollider>();
	if (colliders.size() > 1)
	{
		for (auto collider = colliders.begin(); collider < colliders.end() - 1; collider++)
		{
			Vec2 overlap = Vec2();
			auto collider1 = *collider;
			auto collider2 = *(collider + 1);

			collider1->DebugDraw(1.0f, 0.0f, 0.0f);
			collider2->DebugDraw(0.0f, 0.0f, 1.0f);

			if (CBoxCollider::CheckCollision(collider1, collider2, overlap))
			{
				collider1->OnCollision(collider2, overlap);
				collider2->OnCollision(collider1, overlap);
			}
		}
	}
}



//--- Game Logic Methods ---//
void STest::PrintCollision(CBoxCollider* _a, CBoxCollider* _b, Vec2& _overlap)
{
	std::string msg = _a->GetEntity()->GetName() + " collided with " + _b->GetEntity()->GetName() + " with overlap of " + std::to_string(_overlap.GetX()) + "," + std::to_string(_overlap.GetY());
	App::Print(100, 100, msg.c_str());
}

void STest::SpinPlayer(CBoxCollider* _a, CBoxCollider* _b, Vec2& _overlap)
{
	auto transform = m_player->GetComponent<CTransform>();
	transform->SetRotation(transform->GetRotation() + 1.0f);
}