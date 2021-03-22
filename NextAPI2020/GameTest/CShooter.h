#pragma once

// Standard includes
#include <functional>

// Internal includes
#include "Component.h"
#include "CRadialAimer.h"
#include "Vec2.h"
#include "ProjectileFactory.h"

class CShooter : public Component
{
public:
	//--- Constructors and Destructor ---//
	CShooter();
	virtual ~CShooter();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Methods ---//
	void Shoot();

	//--- Setters ---//
	static void SetProjectileFactory(ProjectileFactory* _projectileFactory);
	void SetFireRate(float _fireRate);
	void SetBulletTag(EntityTag _bulletTag);
	void SetBulletDmg(float _bulletDmg);
	void SetShootEffect(ProjectileShootEffect _shootEffect);
	void SetFlightEffect(ProjectileFlightEffect _flightEffect);
	void SetDestroyEffect(ProjectileDestroyEffect _destroyEffect);

	//--- Getters ---//
	float GetFireRate() const;

private:
	//--- Private Data ---//
	static ProjectileFactory* m_projectileFactory;
	CTransform* m_transform;
	CRadialAimer* m_aimer;
	float m_fireRate;
	float m_timeSinceLastFire;
	EntityTag m_bulletTag;
	float m_bulletDmg;
	ProjectileShootEffect m_shootEffect;
	ProjectileFlightEffect m_flightEffect;
	ProjectileDestroyEffect m_destroyEffect;
};
