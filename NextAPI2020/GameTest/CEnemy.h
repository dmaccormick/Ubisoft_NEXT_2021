#pragma once

// Internal includes
#include "Component.h"
#include "EnemyType.h"

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
	void SetEnemyType(EnemyType _enemyType);

	//--- Getters ---//
	float GetRewardAmount() const;
	float GetDamage() const;
	EnemyType GetEnemyType() const;

private:
	//--- Private Data ---//
	float m_rewardAmount;
	float m_damage;
	EnemyType m_enemyType;
};
