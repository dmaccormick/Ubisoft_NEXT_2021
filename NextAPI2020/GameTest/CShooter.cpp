// Standard header
#include "stdafx.h"

// Internal includes
#include "CShooter.h"

//--- Statics ---//
ProjectileFactory* CShooter::m_projectileFactory = nullptr;



//--- Constructors and Destructor ---//
CShooter::CShooter()
{
	m_turretAimer = nullptr;
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
	m_turretAimer = GetComponent<CRadialAimer>();
}

void CShooter::Update(float _deltaTime)
{
	if (m_turretAimer->GetTarget())
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
	// Call the projectile creation function and pass it the spawner turret and target enemy
	/*if (m_projectilePrefabFunc)
		m_projectilePrefabFunc(GetComponent<CTransform>(), m_turretAimer->GetTarget());*/

	m_projectileFactory->CreateProjectile(m_bulletTag, GetComponent<CTransform>(), m_turretAimer->GetTarget(), m_bulletDmg, m_flightEffect, m_destroyEffect);
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

//void CTurretShooter::SetProjectilePrefabFunc(std::function<void(CTransform*, CTransform*)> _projectilePrefabFunc)
//{
//	m_projectilePrefabFunc = _projectilePrefabFunc;
//}
