#pragma once

// Standard includes
#include <functional>
#include <map>

// Internal includes
#include "Registry.h"
#include "EnemyType.h"
#include "CTransform.h"

struct EnemyDesc
{
	EnemyDesc()
	{
		m_enemyName = "";
		m_moveSpeed = 0.0f;
		m_damage = 0.0f;
		m_health = 0.0f;
		m_reward = 0.0f;
		m_colliderSize = Vec2::Zero();
	}

	EnemyDesc(std::string _enemyName, float _moveSpeed, float _damage, float _health, float _reward, Vec2 _colliderSize)
	{
		m_enemyName = _enemyName;
		m_moveSpeed = _moveSpeed;
		m_damage = _damage;
		m_health = _health;
		m_reward = _reward;
		m_colliderSize = _colliderSize;
	}

	std::string m_enemyName;
	float m_moveSpeed;
	float m_damage;
	float m_health;
	float m_reward;
	Vec2 m_colliderSize;
};

class EnemyFactory
{
public:
	//--- Constructors and Destructor ---//
	EnemyFactory();
	EnemyFactory(CTransform* _enemySpawnerLoc, Registry& _levelRegistry, std::function<void(Entity*)> _inGameKillCallback, std::function<void(Entity*)> _waveKillCallback);
	~EnemyFactory();

	//--- Enemy Prefab Methods ---//
	Entity* CreateEnemy(EnemyType _enemyType) const;

private:
	//--- Private Data ---//
	CTransform* m_enemySpawnerLoc;
	Registry* m_levelRegistry;
	std::function<void(Entity*)> m_gameOnKillCallback;
	std::function<void(Entity*)> m_spawnerOnKillCallback;
	std::unordered_map<EnemyType, EnemyDesc> m_enemyDescs;

	//--- Enemy Prefab Methods ---//
	Entity* CreateGenericEnemy(EnemyType _enemyType) const;
	Entity* CreateBasicEnemy() const;
	Entity* CreateFastEnemy() const;
	Entity* CreateStrongEnemy() const;
	Entity* CreateHealerEnemy() const;
	Entity* CreateSpeederEnemy() const;
	Entity* CreateShooterEnemy() const;
	Entity* CreateEMPEnemy() const;
	Entity* CreateBomberEnemy() const;
	Entity* CreateBossEnemy() const;
};