#pragma once

// Standard includes
#include <functional>

// Internal includes
#include "Component.h"
#include "CRadialAimer.h"
#include "Vec2.h"

class CTurretShooter : public Component
{
public:
	//--- Constructors and Destructor ---//
	CTurretShooter();
	virtual ~CTurretShooter();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Methods ---//
	void Shoot();

	//--- Setters ---//
	void SetFireRate(float _fireRate);
	void SetProjectilePrefabFunc(std::function<void(CTransform*, CTransform*)> _projectilePrefabFunc);

private:
	//--- Private Data ---//
	CRadialAimer* m_turretAimer;
	float m_fireRate;
	float m_timeSinceLastFire;
	std::function<void(CTransform*, CTransform*)> m_projectilePrefabFunc;
};
