#pragma once

// Internal includes
#include "Component.h"

class CProjectile : public Component
{
public:
	//--- Constructors and Destructor ---//
	CProjectile();
	virtual ~CProjectile();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Setters ---//
	void SetDamage(float _damage);

	//--- Getters ---//
	float GetDamage() const;

private:
	//--- Private Data ---//
	float m_damage;
};
