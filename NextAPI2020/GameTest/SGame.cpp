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
#include "CHealth.h"
#include "CButton.h"
#include "CLabel.h"
#include "CBank.h"



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
	LoadLevel();
	SetupPlayer();
	
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
	CTurretAimer::SetEnemyList(m_enemies);

	m_playerHealthLabel->SetText("HEALTH: " + std::to_string(m_playerHealth->GetHealthRounded()));
	m_playerMoneyLabel->SetText("MONEY: " + std::to_string(m_playerBank->GetMoneyRounded()));
}

void SGame::Draw()
{
	// Sort the sprites by draw order and then render them all
	std::vector<CSprite*> sprites = m_registry.GetAllComponentsByType<CSprite>();
	std::sort(sprites.begin(), sprites.end(), [](CSprite* _a, CSprite* _b) {return _a->GetRenderLayer() > _b->GetRenderLayer(); });
	for (auto sprite : sprites)
		sprite->DrawSprite();

	// Draw all of the buttons
	std::vector<CButton*> buttons = m_registry.GetAllComponentsByType<CButton>();
	for (auto button : buttons)
		button->Draw();

	// Draw all of the labels
	std::vector<CLabel*> labels = m_registry.GetAllComponentsByType<CLabel>();
	for (auto label : labels)
		label->Draw();
}



//--- Game Logic Methods ---//
void SGame::LoadLevel()
{
	// Load the level given the parameters
	LevelInfo levelInfo;
	levelInfo.m_topLeftLoc = Vec2(300.0f, 600.0f);
	levelInfo.m_levelDataPath = ".\\GameData\\Levels\\" + m_levelName;
	levelInfo.m_levelTilesetPath = ".\\GameData\\Sprites\\Level_Tiles_1.bmp";
	levelInfo.m_tileSize = 64.0f;

	LevelLoader loader = LevelLoader();
	loader.LoadLevel(levelInfo, m_registry, m_levelPieces);

	// Make all of the tower build locations interactable so that the player can build on them
	std::vector<Entity*> towerBuildLocations = m_registry.GetAllEntitiesByTags({ EntityTag::TurretBuildArea });
	for (auto towerBuildLoc : towerBuildLocations)
	{
		CButton* button = m_registry.AddComponent<CButton>(towerBuildLoc);
		button->SetDimensions(Vec2(32.0f, 32.0f));
		button->AddOnClickedCallback(std::bind(&SGame::PlaceTower, this, P_ARG::_1));
		button->SetColor(Color::Yellow());
		button->Init();

		CLabel* label = m_registry.AddComponent<CLabel>(towerBuildLoc);
		label->SetText("Build");
		label->SetOffset(Vec2(-20.0f, -5.0f));
		label->Init();
		label->SetColor(Color::Yellow());
		label->SetFont(Font::BASE_8_BY_13);
	}
}

void SGame::SetupPlayer()
{
	m_player = m_registry.CreateEntity("Player", EntityTag::Player);
	
	CTransform* transformComp = m_registry.AddComponent<CTransform>(m_player);

	m_playerHealth = m_registry.AddComponent<CHealth>(m_player);
	m_playerHealth->SetMaxHealth(500.0f);
	m_playerHealth->AddOnDestroyCallback(std::bind(&SGame::GameOver, this, P_ARG::_1));

	m_playerBank = m_registry.AddComponent<CBank>(m_player);
	m_playerBank->SetStartMoney(100.0f);

	m_playerMoneyLabel = m_registry.AddComponent<CLabel>(m_player);
	m_playerMoneyLabel->SetColor(Color::Yellow());
	m_playerMoneyLabel->SetFont(Font::TIMES_ROMAN_24);
	m_playerMoneyLabel->SetOffset(Vec2(332.0f, 100.0f));
	m_playerMoneyLabel->SetText("MONEY: 100");

	m_playerHealthLabel = m_registry.AddComponent<CLabel>(m_player);
	m_playerHealthLabel->SetColor(Color::Magenta());
	m_playerHealthLabel->SetFont(Font::TIMES_ROMAN_24);
	m_playerHealthLabel->SetOffset(Vec2(564.0f, 100.0f));
	m_playerHealthLabel->SetText("HEALTH: 500");
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

	CTransform* transformComp = m_registry.AddComponent<CTransform>(enemy);
	transformComp->SetPosition(m_enemySpawner->GetPosition());
	transformComp->Init();

	CSprite* spriteComp = m_registry.AddComponent<CSprite>(enemy);
	spriteComp->LoadSprite(".\\GameData\\Sprites\\Enemy_Base.bmp");
	spriteComp->SetRenderLayer(0.0f);
	spriteComp->Init();

	CBoxCollider* boxCollider = m_registry.AddComponent<CBoxCollider>(enemy);
	boxCollider->SetBaseDimensions(Vec2(5.0f, 5.0f));
	boxCollider->Init();

	CLinearMover* linearMoverComp = m_registry.AddComponent<CLinearMover>(enemy);
	linearMoverComp->SetMovementSpeed(50.0f);
	linearMoverComp->Init();

	CHealth* healthComp = m_registry.AddComponent<CHealth>(enemy);
	healthComp->SetMaxHealth(25.0f);
	healthComp->Init();
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

	if (enemy->GetTag() != EntityTag::Enemy)
		return;

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

	// Damage the enemy
	float bulletDmg = bullet->GetComponent<CProjectile>()->GetDamage();
	enemy->GetComponent<CHealth>()->Damage(bulletDmg);

	m_playerBank->AddMoney(200.0f);
	m_playerHealth->Heal(30.0f);

	// Delete the bullet since it collided with the enemy
	m_registry.DeleteEntity(bullet);
}

void SGame::PlaceTower(Entity* _callingButton)
{
	// Build a tower on top of the build location
	auto turret = m_registry.CreateEntity("Turret");

	CTransform* transformComp = m_registry.AddComponent<CTransform>(turret);
	transformComp->SetPosition(_callingButton->GetComponent<CTransform>()->GetPosition());
	transformComp->Init();

	CSprite* spriteComp = m_registry.AddComponent<CSprite>(turret);
	spriteComp->LoadSprite(".\\GameData\\Sprites\\Turret_Basic.bmp");
	spriteComp->SetRenderLayer(-1.0f);
	spriteComp->Init();

	CTurretAimer* turretAimerComp = m_registry.AddComponent<CTurretAimer>(turret);
	turretAimerComp->SetRange(300.0f);
	turretAimerComp->Init();

	CTurretShooter* shooterComp = m_registry.AddComponent<CTurretShooter>(turret);
	shooterComp->SetFireRate(0.5f);
	shooterComp->SetProjectilePrefabFunc(std::bind(&SGame::FireBasicProjectile, this, P_ARG::_1, P_ARG::_2));
	shooterComp->Init();

	// Disable the button now that the tower has been placed
	_callingButton->GetComponent<CButton>()->SetActive(false);
	_callingButton->GetComponent<CLabel>()->SetActive(false);
}

void SGame::GameOver(Entity* _playerEntity)
{
	// TODO: Load the game over scene
}


//--- Setters ---//
void SGame::SetLevelName(std::string _levelName)
{
	m_levelName = _levelName;
}