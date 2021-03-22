// Standard header
#include "stdafx.h"

// Standard includes
#include <cmath>

// Internal includes
#include "CShooter.h"

//--- Statics ---//
ProjectileFactory* CShooter::m_projectileFactory = nullptr;



//--- Constructors and Destructor ---//
CShooter::CShooter()
{
	m_aimer = nullptr;
	m_fireRate = 0.0f;
	m_timeSinceLastFire = 0.0f;
	m_bulletTag = EntityTag::BulletPlayer;
	m_bulletDmg = 50.0f;
	m_shootEffect = ProjectileShootEffect::Basic;
	m_flightEffect = ProjectileFlightEffect::Basic;
	m_destroyEffect = ProjectileDestroyEffect::Basic;
}

CShooter::~CShooter()
{
}



//--- Component Interface ---//
void CShooter::Init()
{
	m_transform = GetComponent<CTransform>();
	m_aimer = GetComponent<CRadialAimer>();
}

void CShooter::Update(float _deltaTime)
{
	if (m_aimer->GetTarget())
	{
		float dtSeconds = _deltaTime / 1000.0f;

		m_timeSinceLastFire += dtSeconds;
		if (m_timeSinceLastFire >= m_fireRate)
		{
			Shoot();
			m_timeSinceLastFire = 0.0f;
		}
	}
}



//--- Methods ---//
void CShooter::Shoot()
{
	Vec2 shooterToTarget = Vec2(m_aimer->GetTarget()->GetPosition()) - m_transform->GetPosition();
	m_projectileFactory->CreateProjectile(m_bulletTag, m_transform, m_aimer->GetTarget(), shooterToTarget, m_bulletDmg, m_flightEffect, m_destroyEffect);

	if (m_shootEffect == ProjectileShootEffect::TriShot)
	{
		// Spread the shots out
		float spreadAngle = 30.0f * (3.14159f / 180.0f);

		// Rotate the straight on vector to get the two the additional projectile paths
		float cosAngle = cos(spreadAngle);
		float sinAngle = sin(spreadAngle);
		Vec2 positiveSpreadVec;
		positiveSpreadVec.SetX(shooterToTarget.GetX() * cosAngle - shooterToTarget.GetY() * sinAngle);
		positiveSpreadVec.SetY(shooterToTarget.GetX() * sinAngle + shooterToTarget.GetY() * cosAngle);

		float cosAngleNeg = cos(-spreadAngle);
		float sinAngleNeg = sin(-spreadAngle);
		Vec2 negativeSpreadVec;
		negativeSpreadVec.SetX(shooterToTarget.GetX() * cosAngleNeg - shooterToTarget.GetY() * sinAngleNeg);
		negativeSpreadVec.SetY(shooterToTarget.GetX() * sinAngleNeg + shooterToTarget.GetY() * cosAngleNeg);

		// Spawn the two additional projectiles
		m_projectileFactory->CreateProjectile(m_bulletTag, m_transform, m_aimer->GetTarget(), positiveSpreadVec, m_bulletDmg, m_flightEffect, m_destroyEffect);
		m_projectileFactory->CreateProjectile(m_bulletTag, m_transform, m_aimer->GetTarget(), negativeSpreadVec, m_bulletDmg, m_flightEffect, m_destroyEffect);
	}
}



//--- Setters ---//
void CShooter::SetProjectileFactory(ProjectileFactory* _projectileFactory)
{
	m_projectileFactory = _projectileFactory;
}

void CShooter::SetFireRate(float _fireRate)
{
	m_fireRate = _fireRate;
}

void CShooter::SetBulletTag(EntityTag _bulletTag)
{
	m_bulletTag = _bulletTag;
}

void CShooter::SetBulletDmg(float _bulletDmg)
{
	m_bulletDmg = _bulletDmg;
}

void CShooter::SetShootEffect(ProjectileShootEffect _shootEffect)
{
	m_shootEffect = _shootEffect;
}

void CShooter::SetFlightEffect(ProjectileFlightEffect _flightEffect)
{
	m_flightEffect = _flightEffect;
}

void CShooter::SetDestroyEffect(ProjectileDestroyEffect _destroyEffect)
{
	m_destroyEffect = _destroyEffect;
}



//--- Getters ---//
float CShooter::GetFireRate() const
{
	return m_fireRate;
}