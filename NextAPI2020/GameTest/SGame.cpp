// Standard header
#include "stdafx.h"

// Internal includes
#include "App/app.h"
#include "SGame.h"
#include "LevelLoader.h"
#include "CSprite.h"
#include "CBoxCollider.h"
#include "CLinearMover.h"
#include "CProjectile.h"
#include "CTurretShooter.h"
#include "CLifetime.h"



//--- Constructors and Destructor ---//
SGame::SGame()
{
	m_levelName = "Level_1.txt";
	m_levelPieces = std::vector<Entity*>();
	m_timeBetweenEnemies = 1.0f;
	m_timeSinceLastEnemy = m_timeBetweenEnemies;
}

SGame::~SGame()
{

}



//--- Scene Interface ---//
void SGame::Init()
{
	// Create the test turret entity
	{
		auto turret = m_registry.CreateEntity("Turret");

		CTransform* transformComp = m_registry.AddComponent<CTransform>(turret);
		transformComp->SetPosition(400.0f, 400.0f);
		transformComp->Init();

		CSprite* spriteComp = m_registry.AddComponent<CSprite>(turret);
		spriteComp->LoadSprite(".\\GameData\\Sprites\\Turret_Basic.bmp");
		spriteComp->SetRenderLayer(-1.0f);
		spriteComp->Init();

		m_testTurret = m_registry.AddComponent<CTurretAimer>(turret);
		m_testTurret->SetRange(300.0f);

		CTurretShooter* shooterComp = m_registry.AddComponent<CTurretShooter>(turret);
		shooterComp->SetFireRate(0.5f);
		shooterComp->SetProjectilePrefabFunc(std::bind(&SGame::FireBasicProjectile, this, P_ARG::_1, P_ARG::_2));
	}

	// Create the test turret entity
	{
		auto turret = m_registry.CreateEntity("Turret");

		CTransform* transformComp = m_registry.AddComponent<CTransform>(turret);
		transformComp->SetPosition(600.0f, 500.0f);
		transformComp->Init();

		CSprite* spriteComp = m_registry.AddComponent<CSprite>(turret);
		spriteComp->LoadSprite(".\\GameData\\Sprites\\Turret_Basic.bmp");
		spriteComp->SetRenderLayer(-1.0f);
		spriteComp->Init();

		m_testTurret2 = m_registry.AddComponent<CTurretAimer>(turret);
		m_testTurret2->SetRange(300.0f);

		CTurretShooter* shooterComp = m_registry.AddComponent<CTurretShooter>(turret);
		shooterComp->SetFireRate(0.5f);
		shooterComp->SetProjectilePrefabFunc(std::bind(&SGame::FireBasicProjectile, this, P_ARG::_1, P_ARG::_2));
	}

	LoadLevel();
	
	m_registry.InitAll();

	m_enemySpawner = m_registry.GetAllEntitiesByTags({ EntityTag::EnemySpawn })[0]->GetComponent<CTransform>();

	// Register all of the enemy path objects collision callbacks so the enemies can be notified of when to change movement direction
	std::vector<CBoxCollider*> pathColliders = m_registry.GetAllComponentsByTypeAndTags<CBoxCollider>({ EntityTag::EnemySpawn, EntityTag::EnemyPath });
	for (auto collider : pathColliders)
		collider->AddCollisionListener(std::bind(&SGame::TriggerEnemyDirectionChange, this, P_ARG::_1, P_ARG::_2, P_ARG::_3));

	// Register the player base collision callback so the enemies can be destroyed and the player can take the appropriate damage
	CBoxCollider* playerBase = m_registry.GetAllComponentsByTypeAndTags<CBoxCollider>({ EntityTag::PlayerBase })[0];
	playerBase->AddCollisionListener(std::bind(&SGame::AttackPlayerBase, this, P_ARG::_1, P_ARG::_2, P_ARG::_3));
}

void SGame::Update(float _deltaTime)
{
	float dtSeconds = _deltaTime / 1000.0f;

	m_registry.UpdateAll(_deltaTime);

	CheckCollisions();

	m_timeSinceLastEnemy += dtSeconds;
	if (m_timeSinceLastEnemy >= m_timeBetweenEnemies)
	{
		m_timeSinceLastEnemy = 0.0f;
		SpawnEnemy();
	}

	m_enemies = m_registry.GetAllEntitiesByTags({ EntityTag::Enemy });
	m_testTurret->SetEnemyList(m_enemies);
	m_testTurret2->SetEnemyList(m_enemies);
}

void SGame::Draw()
{
	// Sort the sprites by draw order and then render them all
	std::vector<CSprite*> sprites = m_registry.GetAllComponentsByType<CSprite>();
	std::sort(sprites.begin(), sprites.end(), [](CSprite* _a, CSprite* _b) {return _a->GetRenderLayer() > _b->GetRenderLayer(); });

	for (auto sprite : sprites)
		sprite->DrawSprite();

	auto colliders = m_registry.GetAllComponentsByType<CBoxCollider>();
	for (auto collider : colliders)
		collider->DebugDraw();
}



//--- Game Logic Methods ---//
void SGame::LoadLevel()
{
	LevelInfo levelInfo;
	levelInfo.m_topLeftLoc = Vec2(300.0f, 600.0f);
	levelInfo.m_levelDataPath = ".\\GameData\\Levels\\" + m_levelName;
	levelInfo.m_levelTilesetPath = ".\\GameData\\Sprites\\Level_Tiles_1.bmp";
	levelInfo.m_tileSize = 64.0f;

	LevelLoader loader = LevelLoader();
	loader.LoadLevel(levelInfo, m_registry, m_levelPieces);
}

void SGame::CheckCollisions()
{
	// Grab all of the colliders and check for collisions between them
	std::vector<CBoxCollider*> colliders = m_registry.GetAllComponentsByType<CBoxCollider>();
	if (colliders.size() > 1)
	{
		for (unsigned int i = 0; i < colliders.size(); i++)
		{
			for (unsigned int j = 0; j < colliders.size(); j++)
			{
				auto collider1 = &colliders[i];
				auto collider2 = &colliders[j];

				if (*collider1 == *collider2)
					continue;

				Vec2 overlap;
				if (CBoxCollider::CheckCollision(*collider1, *collider2, overlap))
				{
					(*collider1)->OnCollision(*collider2, overlap);
					(*collider2)->OnCollision(*collider1, overlap);
				}
			}
		}
	}
}

void SGame::SpawnEnemy()
{
	auto enemy = m_registry.CreateEntity("Enemy", EntityTag::Enemy);

	auto transformComp = m_registry.AddComponent<CTransform>(enemy);
	transformComp->SetPosition(m_enemySpawner->GetPosition());
	transformComp->Init();

	auto spriteComp = m_registry.AddComponent<CSprite>(enemy);
	spriteComp->LoadSprite(".\\GameData\\Sprites\\Enemy_Base.bmp");
	spriteComp->SetRenderLayer(0.0f);
	spriteComp->Init();

	auto boxCollider = m_registry.AddComponent<CBoxCollider>(enemy);
	boxCollider->SetBaseDimensions(Vec2(5.0f, 5.0f));
	boxCollider->Init();

	auto pathFollowerComp = m_registry.AddComponent<CLinearMover>(enemy);
	pathFollowerComp->SetMovementSpeed(50.0f);
	pathFollowerComp->Init();
}

void SGame::TriggerEnemyDirectionChange(CBoxCollider* _a, CBoxCollider* _b, Vec2& _overlap)
{
	// Figure out which object is the enemy and which is the path direction changer
	Entity* enemy = (_a->GetEntity()->GetTag() == EntityTag::Enemy) ? _a->GetEntity() : _b->GetEntity();
	Entity* directionChanger = (enemy == _a->GetEntity()) ? _b->GetEntity() : _a->GetEntity();

	if (enemy->GetTag() != EntityTag::Enemy)
		return;

	// Tell the enemy to start moving in the new direction
	Vec2 newDirection = directionChanger->GetComponent<CTile>()->GetMovementDirection();
	enemy->GetComponent<CLinearMover>()->SetMovementDirection(newDirection);
}

void SGame::AttackPlayerBase(CBoxCollider* _a, CBoxCollider* _b, Vec2& _overlap)
{
	// Figure out which object is the enemy
	Entity* enemy = (_a->GetEntity()->GetTag() == EntityTag::Enemy) ? _a->GetEntity() : _b->GetEntity();

	// Delete the enemy since it has reached the player base
	m_registry.DeleteEntity(enemy);

	// TODO: Cause damage to the player
	// ...
}

void SGame::FireBasicProjectile(CTransform* _turret, CTransform* _enemy)
{
	// Spawn the most basic type of projectile which just flies in a straight line
	auto basicProjectile = m_registry.CreateEntity("BasicProjectile", EntityTag::BulletPlayer);

	CTransform* transformComp = m_registry.AddComponent<CTransform>(basicProjectile);
	transformComp->SetPosition(_turret->GetPosition());
	transformComp->Init();

	CSprite* spriteComp = m_registry.AddComponent<CSprite>(basicProjectile);
	spriteComp->LoadSprite(".\\GameData\\Sprites\\Projectile_Base.bmp");
	spriteComp->SetRenderLayer(-5.0f);
	spriteComp->Init();

	CLinearMover* linearMoverComp = m_registry.AddComponent<CLinearMover>(basicProjectile);
	linearMoverComp->SetMovementSpeed(300.0f);
	linearMoverComp->SetMovementDirection(_enemy->GetPosition() - _turret->GetPosition());
	linearMoverComp->Init();

	CBoxCollider* boxColliderComp = m_registry.AddComponent<CBoxCollider>(basicProjectile);
	boxColliderComp->SetBaseDimensions(10.0f, 10.0f);
	boxColliderComp->AddCollisionListener(std::bind(&SGame::DamageEnemy, this, P_ARG::_1, P_ARG::_2, P_ARG::_3));
	boxColliderComp->Init();

	CProjectile* projectileComp = m_registry.AddComponent<CProjectile>(basicProjectile);
	projectileComp->SetDamage(10.0f);
	projectileComp->Init();

	CLifetime* lifetimeComp = m_registry.AddComponent<CLifetime>(basicProjectile);
	lifetimeComp->SetMaxLifetime(5.0f);
	lifetimeComp->Init();
}

void SGame::DamageEnemy(CBoxCollider* _a, CBoxCollider* _b, Vec2& _overlap)
{
	// Figure out which object is the enemy and which is the bullet
	Entity* enemy = (_a->GetEntity()->GetTag() == EntityTag::Enemy) ? _a->GetEntity() : _b->GetEntity();
	Entity* bullet = (enemy == _a->GetEntity()) ? _b->GetEntity() : _a->GetEntity();

	if (enemy->GetTag() != EntityTag::Enemy || bullet->GetTag() != EntityTag::BulletPlayer)
		return;

	// TODO: Damage the enemy
	// ...

	// Delete the bullet since it collided with the enemy
	m_registry.DeleteEntity(bullet);
}


//--- Setters ---//
void SGame::SetLevelName(std::string _levelName)
{
	m_levelName = _levelName;
}