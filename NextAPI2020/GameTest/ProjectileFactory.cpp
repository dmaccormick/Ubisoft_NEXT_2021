// Standard header
#include "stdafx.h"

// Internal includes
#include "ProjectileFactory.h"
#include "CSprite.h"
#include "CBoxCollider.h"
#include "CLifetime.h"
#include "CProjectile.h"
#include "CLinearMover.h"

//--- Constructors and Destructor ---//
ProjectileFactory::ProjectileFactory(Registry& _levelRegistry, std::function<void(CBoxCollider*, CBoxCollider*, Vec2&)> _gameCollisionCallback)
{
	m_levelRegistry = &_levelRegistry;
	m_gameCollisionCallback = _gameCollisionCallback;
}

ProjectileFactory::~ProjectileFactory()
{
}



//--- Methods ---//
Entity* ProjectileFactory::CreateProjectile(EntityTag _bulletTag, CTransform* _turret, CTransform* _target, float _damage, ProjectileFlightEffect _flightEffect, ProjectileDestroyEffect _destroyEffect)
{
	auto projectile = m_levelRegistry->CreateEntity("BasicProjectile", _bulletTag);

	CTransform* transformComp = m_levelRegistry->AddComponent<CTransform>(projectile);
	transformComp->SetPosition(_turret->GetPosition());
	transformComp->Init();

	CSprite* spriteComp = m_levelRegistry->AddComponent<CSprite>(projectile);
	spriteComp->LoadSprite(".\\GameData\\Sprites\\Projectiles.bmp", (int)ProjectileFlightEffect::Count);
	spriteComp->SetFrame((int)_flightEffect);
	spriteComp->SetRenderLayer(-5.0f);
	spriteComp->Init();

	CBoxCollider* boxColliderComp = m_levelRegistry->AddComponent<CBoxCollider>(projectile);
	boxColliderComp->SetBaseDimensions(20.0f, 20.0f);
	boxColliderComp->AddCollisionListener(m_gameCollisionCallback);
	boxColliderComp->Init();

	CLifetime* lifetimeComp = m_levelRegistry->AddComponent<CLifetime>(projectile);
	lifetimeComp->SetMaxLifetime(5.0f);
	lifetimeComp->Init();

	CProjectile* projectileComp = m_levelRegistry->AddComponent<CProjectile>(projectile);
	projectileComp->SetDamage(10.0f);
	projectileComp->Init();

	// Configure the flight style
	if (_flightEffect == ProjectileFlightEffect::Basic)
	{
		CLinearMover* linearMoverComp = m_levelRegistry->AddComponent<CLinearMover>(projectile);
		linearMoverComp->SetMovementSpeed(500.0f);
		linearMoverComp->SetMovementDirection(_target->GetPosition() - _turret->GetPosition());
		linearMoverComp->Init();
	}
	else
	{
		// TODO: Add homing
	}

	// Configure the destruction effect
	if (_destroyEffect == ProjectileDestroyEffect::Splash)
	{
		// TODO: Add splash
	}

	return projectile;
}
