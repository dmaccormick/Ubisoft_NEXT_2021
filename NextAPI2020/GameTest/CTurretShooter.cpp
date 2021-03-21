// Standard header
#include "stdafx.h"

// Internal includes
#include "CTurretShooter.h"

//--- Constructors and Destructor ---//
CTurretShooter::CTurretShooter()
{
	m_turretAimer = nullptr;
	m_fireRate = 0.0f;
	m_timeSinceLastFire = 0.0f;
}

CTurretShooter::~CTurretShooter()
{
}



//--- Component Interface ---//
void CTurretShooter::Init()
{
	m_turretAimer = GetComponent<CRadialAimer>();
}

void CTurretShooter::Update(float _deltaTime)
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
void CTurretShooter::Shoot()
{
	// Call the projectile creation function and pass it the spawner turret and target enemy
	if (m_projectilePrefabFunc)
		m_projectilePrefabFunc(GetComponent<CTransform>(), m_turretAimer->GetTarget());
}



//--- Setters ---//
void CTurretShooter::SetFireRate(float _fireRate)
{
	m_fireRate = _fireRate;
}

void CTurretShooter::SetProjectilePrefabFunc(std::function<void(CTransform*, CTransform*)> _projectilePrefabFunc)
{
	m_projectilePrefabFunc = _projectilePrefabFunc;
}
