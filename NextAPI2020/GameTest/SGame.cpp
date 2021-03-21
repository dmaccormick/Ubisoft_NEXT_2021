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
#include "CEnemy.h"
#include "SEndScreen.h"
#include "SceneManager.h"
#include "SMenu.h"
#include "CRadiusIndicator.h"

//--- Statics ---//
std::string SGame::m_levelName = "Level_1.txt";



//--- Constructors and Destructor ---//
SGame::SGame()
{
	m_levelPieces = std::vector<Entity*>();
	m_timeBetweenEnemies = 1.0f;
	m_timeSinceLastEnemy = m_timeBetweenEnemies;
	m_turretBuildCost = 100.0f;
	m_victoryState = VictoryState::StillPlaying;
	m_quitToMenu = false;
}

SGame::~SGame()
{
	delete m_waveSpawner;
}



//--- Scene Interface ---//
void SGame::Init()
{
	LoadLevel();
	CreateQuitToMenuButton();
	SetupPlayer();
	
	m_registry.InitAll();

	// Register all of the enemy path objects collision callbacks so the enemies can be notified of when to change movement direction
	std::vector<CBoxCollider*> pathColliders = m_registry.GetAllComponentsByTypeAndTags<CBoxCollider>({ EntityTag::EnemySpawn, EntityTag::EnemyPath });
	for (auto collider : pathColliders)
		collider->AddCollisionListener(std::bind(&SGame::TriggerEnemyDirectionChange, this, P_ARG::_1, P_ARG::_2, P_ARG::_3));

	// Register the player base collision callback so the enemies can be destroyed and the player can take the appropriate damage
	CBoxCollider* playerBase = m_registry.GetAllComponentsByTypeAndTags<CBoxCollider>({ EntityTag::PlayerBase })[0];
	playerBase->AddCollisionListener(std::bind(&SGame::AttackPlayerBase, this, P_ARG::_1, P_ARG::_2, P_ARG::_3));

	// Init the wave spawner
	auto enemySpawner = m_registry.GetAllEntitiesByTags({ EntityTag::EnemySpawn })[0]->GetComponent<CTransform>();
	m_waveSpawner = new WaveSpawner(enemySpawner, m_registry, std::bind(&SGame::KillEnemy, this, P_ARG::_1));
	m_waveSpawner->StartNextWave();
}

void SGame::Update(float _deltaTime)
{
	// Convert the delta time from ms to s
	float dtSeconds = _deltaTime / 1000.0f;

	// Update all of the components in the registry`
	m_registry.UpdateAll(_deltaTime);

	// Update the player UI
	m_playerHealthLabel->SetText("HEALTH: " + std::to_string(m_playerHealth->GetHealthRounded()));
	m_playerMoneyLabel->SetText("MONEY: " + std::to_string(m_playerBank->GetMoneyRounded()));
	m_waveNumberLabel->SetText("WAVE: " + std::to_string(m_waveSpawner->GetCurrentWaveNumber()) + "/" + std::to_string(m_waveSpawner->GetNumWaves()));
	m_enemiesLeftLabel->SetText("ENEMIES: " + std::to_string(m_waveSpawner->GetEnemyCountRemaining()));

	// Look for and handle any collisions
	CheckCollisions();

	// Update the wave spawner
	m_waveSpawner->Update(_deltaTime);

	// Update the list of targets for the turrets to focus on
	m_enemies = m_registry.GetAllEntitiesByTags({ EntityTag::Enemy });
	CTurretAimer::SetEnemyList(m_enemies);

	// If all of the waves have been defeated, the player has won
	if (m_waveSpawner->GetAllWavesComplete())
		m_victoryState = VictoryState::Victory;

	// Load the end screen if the game has finished
	// Go back to the main menu if the button was pressed
	if (m_victoryState != VictoryState::StillPlaying)
	{
		SEndScreen::SetVictoryState(m_victoryState);
		SceneManager::GetInstance()->LoadScene<SEndScreen>();
	}
	else if (m_quitToMenu)
	{
		SceneManager::GetInstance()->LoadScene<SMenu>();
	}
}

void SGame::Draw()
{
	// Sort the sprites by draw order and then render them all
	std::vector<CSprite*> sprites = m_registry.GetAllComponentsByType<CSprite>();
	std::sort(sprites.begin(), sprites.end(), [](CSprite* _a, CSprite* _b) {return _a->GetRenderLayer() > _b->GetRenderLayer(); });
	for (auto sprite : sprites)
		sprite->Draw();

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
		button->AddOnClickedCallback(std::bind(&SGame::PlaceTurret, this, P_ARG::_1));
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
	m_playerBank->SetStartMoney(500.0f);

	m_playerMoneyLabel = m_registry.AddComponent<CLabel>(m_player);
	m_playerMoneyLabel->SetColor(Color::Yellow());
	m_playerMoneyLabel->SetFont(Font::TIMES_ROMAN_24);
	m_playerMoneyLabel->SetOffset(Vec2(332.0f, 95.0f));
	m_playerMoneyLabel->SetText("MONEY: 100");

	m_playerHealthLabel = m_registry.AddComponent<CLabel>(m_player);
	m_playerHealthLabel->SetColor(Color::Magenta());
	m_playerHealthLabel->SetFont(Font::TIMES_ROMAN_24);
	m_playerHealthLabel->SetOffset(Vec2(564.0f, 95.0f));
	m_playerHealthLabel->SetText("HEALTH: 500");

	m_waveNumberLabel = m_registry.AddComponent<CLabel>(m_player);
	m_waveNumberLabel->SetColor(Color::Cyan());
	m_waveNumberLabel->SetFont(Font::TIMES_ROMAN_24);
	m_waveNumberLabel->SetOffset(Vec2(332.0f, 650.0f));
	m_waveNumberLabel->SetText("WAVE: 1/3");

	m_enemiesLeftLabel = m_registry.AddComponent<CLabel>(m_player);
	m_enemiesLeftLabel->SetColor(Color::Cyan());
	m_enemiesLeftLabel->SetFont(Font::TIMES_ROMAN_24);
	m_enemiesLeftLabel->SetOffset(Vec2(564.0f, 650.0f));
	m_enemiesLeftLabel->SetText("ENEMIES: 10");
}

void SGame::CreateQuitToMenuButton()
{
	// Place the quit to menu button
	auto menuButton = m_registry.CreateEntity("MenuButton");

	CTransform* transformComp = m_registry.AddComponent<CTransform>(menuButton);
	transformComp->SetPosition(75.0f, 710.0f);

	CButton* buttonComp = m_registry.AddComponent<CButton>(menuButton);
	buttonComp->SetDimensions(Vec2(100.0f, 50.0f));
	buttonComp->SetColor(Color::Red());
	buttonComp->AddOnClickedCallback(std::bind(&SGame::QuitToMenu, this, P_ARG::_1));

	CLabel* labelComp = m_registry.AddComponent<CLabel>(menuButton);
	labelComp->SetText("QUIT TO MENU");
	labelComp->SetColor(Color::Red());
	labelComp->SetOffset(Vec2(-40.0f, -5.0f));
	labelComp->SetFont(Font::HELVETICA_10);
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

	// Cause damage to the player
	m_playerHealth->Damage(enemy->GetComponent<CEnemy>()->GetDamage());

	// Delete the enemy since it has reached the player base
	m_registry.DeleteEntity(enemy);
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
	linearMoverComp->SetMovementSpeed(500.0f);
	linearMoverComp->SetMovementDirection(_enemy->GetPosition() - _turret->GetPosition());
	linearMoverComp->Init();

	CBoxCollider* boxColliderComp = m_registry.AddComponent<CBoxCollider>(basicProjectile);
	boxColliderComp->SetBaseDimensions(20.0f, 20.0f);
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

	// Delete the bullet since it collided with the enemy
	m_registry.DeleteEntity(bullet);
}

void SGame::KillEnemy(Entity* _enemy)
{
	// Give the player the correct amount of money
	CEnemy* enemyComp = _enemy->GetComponent<CEnemy>();
	m_playerBank->AddMoney(enemyComp->GetRewardAmount());
}

void SGame::PlaceTurret(Entity* _callingButton)
{
	if (m_playerBank->GetMoney() >= m_turretBuildCost)
	{
		// Charge the player the cost to build it
		m_playerBank->RemoveMoney(m_turretBuildCost);

		// Build a tower on top of the build location
		auto turret = m_registry.CreateEntity("Turret", EntityTag::Turret);

		CTransform* transformComp = m_registry.AddComponent<CTransform>(turret);
		transformComp->SetPosition(_callingButton->GetComponent<CTransform>()->GetPosition());
		transformComp->Init();

		CSprite* spriteComp = m_registry.AddComponent<CSprite>(turret);
		spriteComp->LoadSprite(".\\GameData\\Sprites\\Turret_Basic.bmp");
		spriteComp->SetRenderLayer(-1.0f);
		spriteComp->Init();

		CRadiusIndicator* radiusIndicatorComp = m_registry.AddComponent<CRadiusIndicator>(turret);
		radiusIndicatorComp->SetColor(Color::White(0.2f));
		radiusIndicatorComp->SetRenderLayer(-0.5f);
		radiusIndicatorComp->Init();

		CTurretAimer* turretComp = m_registry.AddComponent<CTurretAimer>(turret);
		turretComp->SetRange(100.0f);
		turretComp->Init();

		CTurretShooter* shooterComp = m_registry.AddComponent<CTurretShooter>(turret);
		shooterComp->SetFireRate(0.5f);
		shooterComp->SetProjectilePrefabFunc(std::bind(&SGame::FireBasicProjectile, this, P_ARG::_1, P_ARG::_2));
		shooterComp->Init();

		CHealth* healthComp = m_registry.AddComponent<CHealth>(turret);
		healthComp->SetMaxHealth(100.0f);
		healthComp->Init();

		// Disable the button now that the tower has been placed
		_callingButton->GetComponent<CButton>()->SetActive(false);
		_callingButton->GetComponent<CLabel>()->SetActive(false);
	}
}

void SGame::GameOver(Entity* _playerEntity)
{
	m_victoryState = VictoryState::Loss;
}

void SGame::QuitToMenu(Entity* _callingButton)
{
	m_quitToMenu = true;
}


//--- Setters ---//
void SGame::SetLevelName(std::string _levelName)
{
	m_levelName = _levelName;
}