// Standard header
#include "stdafx.h"

// Internal includes
#include "EnemyFactory.h"
#include "CTransform.h"
#include "CProjectile.h"
#include "CSprite.h"
#include "CBoxCollider.h"
#include "CLifetime.h"
#include "CLinearMover.h"
#include "CHealth.h"
#include "CEnemy.h"

//--- Constructors and Destructor ---//
EnemyFactory::EnemyFactory()
{
	m_enemySpawnerLoc = nullptr;
	m_levelRegistry = nullptr;
	m_gameOnKillCallback = nullptr;
	m_spawnerOnKillCallback = nullptr;
}

EnemyFactory::EnemyFactory(CTransform* _enemySpawnerLoc, Registry& _levelRegistry, std::function<void(Entity*)> _gameOnKillCallback, std::function<void(Entity*)> _spawnerOnKillCallback)
{
	m_enemySpawnerLoc = _enemySpawnerLoc;
	m_levelRegistry = &_levelRegistry;
	m_gameOnKillCallback = _gameOnKillCallback;
	m_spawnerOnKillCallback = _spawnerOnKillCallback;

	m_enemyDescs = {
		{EnemyType::Basic,		EnemyDesc("EnemyBasic",		50.0f,	100.0f, 25.0f,	50.0f, Vec2(5.0f, 5.0f))},

		{EnemyType::Fast,		EnemyDesc("EnemyFast",		75.0f,	50.0f,	15.0f,	75.0f, Vec2(4.0f, 4.0f))},
		{EnemyType::Strong,		EnemyDesc("EnemyStrong",	20.0f,	200.0f, 50.0f,	75.0f, Vec2(8.0f, 8.0f))},

		{EnemyType::Healer,		EnemyDesc("EnemyHealer",	30.0f,	150.0f, 75.0f,	100.0f, Vec2(12.0f, 12.0f))},
		{EnemyType::Speeder,	EnemyDesc("EnemySpeeder",	30.0f,	150.0f, 75.0f,	100.0f, Vec2(12.0f, 12.0f))},
		{EnemyType::Shooter,	EnemyDesc("EnemyShooter",	30.0f,	150.0f, 75.0f,	100.0f, Vec2(12.0f, 12.0f))},
		{EnemyType::EMP,		EnemyDesc("EnemyEMP",		30.0f,	150.0f, 75.0f,	100.0f, Vec2(12.0f, 12.0f))},
		{EnemyType::Bomber,		EnemyDesc("EnemyBomber",	30.0f,	150.0f, 75.0f,	100.0f, Vec2(12.0f, 12.0f))},

		{EnemyType::Boss,		EnemyDesc("EnemyBoss",		10.0f,	500.0f, 500.0f, 500.0f, Vec2(20.0f, 20.0f))}
	};
}

EnemyFactory::~EnemyFactory()
{
}



//--- Methods ---//
Entity* EnemyFactory::CreateEnemy(EnemyType _type) const
{
	switch (_type)
	{
		case EnemyType::Fast:
			return CreateFastEnemy();
		case EnemyType::Strong:
			return CreateStrongEnemy();
			
		case EnemyType::Healer:
			return CreateHealerEnemy();
		case EnemyType::Speeder:
			return CreateSpeederEnemy();
		case EnemyType::Shooter:
			return CreateShooterEnemy();
		case EnemyType::EMP:
			return CreateEMPEnemy();
		case EnemyType::Bomber:
			return CreateBomberEnemy();
		case EnemyType::Boss:
			return CreateBossEnemy();

		case EnemyType::Basic:
		default:
			return CreateBasicEnemy();
	}
}



//--- Enemy Prefab Methods ---//
Entity* EnemyFactory::CreateGenericEnemy(EnemyType _enemyType) const
{
	// Use the enemy descriptor to create the basic foundation of the enemy
	EnemyDesc desc = m_enemyDescs.at(_enemyType);

	auto enemy = m_levelRegistry->CreateEntity(desc.m_enemyName, EntityTag::Enemy);

	CTransform* transformComp = m_levelRegistry->AddComponent<CTransform>(enemy);
	transformComp->SetPosition(m_enemySpawnerLoc->GetPosition());
	transformComp->SetScale(1.5f);
	transformComp->Init();

	CSprite* spriteComp = m_levelRegistry->AddComponent<CSprite>(enemy);
	spriteComp->LoadSprite(".\\GameData\\Sprites\\Enemies.bmp", (int)EnemyType::Count, 1);
	spriteComp->SetFrame((int)_enemyType);
	spriteComp->Init();

	CBoxCollider* boxCollider = m_levelRegistry->AddComponent<CBoxCollider>(enemy);
	boxCollider->SetBaseDimensions(desc.m_colliderSize);
	boxCollider->Init();

	CLinearMover* linearMoverComp = m_levelRegistry->AddComponent<CLinearMover>(enemy);
	linearMoverComp->SetMovementSpeed(desc.m_moveSpeed);
	linearMoverComp->Init();

	CHealth* healthComp = m_levelRegistry->AddComponent<CHealth>(enemy);
	healthComp->SetMaxHealth(desc.m_health);
	healthComp->Init();
	healthComp->AddOnDestroyCallback(m_gameOnKillCallback);
	healthComp->AddOnDestroyCallback(m_spawnerOnKillCallback);

	CEnemy* enemyComp = m_levelRegistry->AddComponent<CEnemy>(enemy);
	enemyComp->SetDamage(desc.m_damage);
	enemyComp->SetRewardAmount(desc.m_reward);
	enemyComp->Init();

	return enemy;
}

Entity* EnemyFactory::CreateBasicEnemy() const
{
	return CreateGenericEnemy(EnemyType::Basic);
}

Entity* EnemyFactory::CreateFastEnemy() const
{
	return CreateGenericEnemy(EnemyType::Fast);
}

Entity* EnemyFactory::CreateStrongEnemy() const
{
	return CreateGenericEnemy(EnemyType::Strong);
}

Entity* EnemyFactory::CreateHealerEnemy() const
{
	Entity* enemy = CreateGenericEnemy(EnemyType::Healer);
	
	return enemy;
}

Entity* EnemyFactory::CreateSpeederEnemy() const
{
	Entity* enemy = CreateGenericEnemy(EnemyType::Speeder);

	return enemy;
}

Entity* EnemyFactory::CreateShooterEnemy() const
{
	Entity* enemy = CreateGenericEnemy(EnemyType::Shooter);

	return enemy;
}

Entity* EnemyFactory::CreateEMPEnemy() const
{
	Entity* enemy = CreateGenericEnemy(EnemyType::EMP);

	return enemy;
}

Entity* EnemyFactory::CreateBomberEnemy() const
{
	Entity* enemy = CreateGenericEnemy(EnemyType::Bomber);

	return enemy;
}

Entity* EnemyFactory::CreateBossEnemy() const
{
	Entity* enemy = CreateGenericEnemy(EnemyType::Boss);

	return enemy;
}
