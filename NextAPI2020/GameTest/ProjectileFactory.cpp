// Standard header
#include "stdafx.h"

// Internal includes
#include "ProjectileFactory.h"
#include "CSprite.h"
#include "CBoxCollider.h"
#include "CLifetime.h"
#include "CProjectile.h"
#include "CLinearMover.h"
#include "CHomingProjectile.h"
#include "CRadiusIndicator.h"
#include "CRadialDamager.h"

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
Entity* ProjectileFactory::CreateProjectile(EntityTag _bulletTag, CTransform* _turret, CTransform* _target, Vec2 _movementDir, float _damage, ProjectileFlightEffect _flightEffect, ProjectileDestroyEffect _destroyEffect)
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

	CLinearMover* linearMoverComp = m_levelRegistry->AddComponent<CLinearMover>(projectile);
	linearMoverComp->SetMovementSpeed(500.0f);
	linearMoverComp->SetMovementDirection(_movementDir);
	linearMoverComp->Init();

	// Optionally add homing
	if (_flightEffect == ProjectileFlightEffect::Homing)
	{
		CHomingProjectile* homingComp = m_levelRegistry->AddComponent<CHomingProjectile>(projectile);
		homingComp->SetTarget(_target);
		homingComp->Init();
	}

	// Optionally add splash damage
	if (_destroyEffect == ProjectileDestroyEffect::Splash)
	{
		CRadiusIndicator* radiusIndicatorComp = m_levelRegistry->AddComponent<CRadiusIndicator>(projectile);
		radiusIndicatorComp->SetColor(Color::Brown());
		radiusIndicatorComp->Init();

		CRadialDamager* radialDamager = m_levelRegistry->AddComponent<CRadialDamager>(projectile);
		radialDamager->SetRadius(30.0f);
		radialDamager->SetTargetEntityTag(EntityTag::Enemy);
		radialDamager->SetDamageAmount(50.0f);
		radialDamager->Init();
	}

	return projectile;
}
