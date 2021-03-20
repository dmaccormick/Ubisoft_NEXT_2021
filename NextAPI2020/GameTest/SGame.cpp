// Standard header
#include "stdafx.h"

// Internal includes
#include "App/app.h"
#include "SGame.h"
#include "LevelLoader.h"
#include "CSprite.h"
#include "CBoxCollider.h"
#include "CPathFollower.h"

//--- Statics ---//
std::string SGame::m_assetFolderPath = ".\\GameData\\";



//--- Constructors and Destructor ---//
SGame::SGame()
{
	m_levelName = "Level_3.txt";
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
	LoadLevel();

	m_enemySpawner = m_registry.GetAllEntitiesByTags({ EntityTag::EnemySpawn })[0]->GetComponent<CTransform>();

	m_registry.InitAll();

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
	levelInfo.m_levelDataPath = m_assetFolderPath + "Levels\\" + m_levelName;
	levelInfo.m_levelTilesetPath = m_assetFolderPath + "Sprites\\Level_Tiles_1.bmp";
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

	auto pathFollowerComp = m_registry.AddComponent<CPathFollower>(enemy);
	pathFollowerComp->SetMovementSpeed(250.0f);
	pathFollowerComp->Init();
}

void SGame::TriggerEnemyDirectionChange(CBoxCollider* _a, CBoxCollider* _b, Vec2& _overlap)
{
	// Figure out which object is the enemy and which is the path direction changer
	Entity* enemy = (_a->GetEntity()->GetTag() == EntityTag::Enemy) ? _a->GetEntity() : _b->GetEntity();
	Entity* directionChanger = (enemy == _a->GetEntity()) ? _b->GetEntity() : _a->GetEntity();

	// Tell the enemy to start moving in the new direction
	Vec2 newDirection = directionChanger->GetComponent<CTile>()->GetMovementDirection();
	enemy->GetComponent<CPathFollower>()->SetMovementDirection(newDirection);
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



//--- Setters ---//
void SGame::SetLevelName(std::string _levelName)
{
	m_levelName = _levelName;
}