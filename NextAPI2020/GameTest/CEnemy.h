#pragma once

// Internal includes
#include "Component.h"

class CEnemy : public Component
{
public:
	//--- Constructors and Destructor ---//
	CEnemy();
	virtual ~CEnemy();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Setters ---//
	void SetRewardAmount(float _reward);
	void SetDamage(float _damage);

	//--- Getters ---//
	float GetRewardAmount() const;
	float GetDamage() const;

private:
	//--- Private Data ---//
	float m_rewardAmount;
	float m_damage;
};
