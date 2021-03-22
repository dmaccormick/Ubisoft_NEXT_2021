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
#include "CShooter.h"
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
#include "CMouseFollower.h"
#include "CRadialHealer.h"
#include "CRadialDamager.h"
#include "EntityRadiusChecker.h"
#include "CRadialSpeeder.h"

//--- Statics ---//
std::string SGame::m_levelName = "Level_1.txt";



//--- Constructors and Destructor ---//
SGame::SGame()
{
	m_levelPieces = std::vector<Entity*>();
	m_timeBetweenEnemies = 1.0f;
	m_timeSinceLastEnemy = m_timeBetweenEnemies;
	m_victoryState = VictoryState::StillPlaying;
	m_quitToMenu = false;
	m_waveSpawner = nullptr;
	m_projectileFactory = nullptr;
	m_currentAbility = PlayerAbility::None;

	m_turretRadius = 100.0f;
	m_turretFireRate = 0.5f;
	m_turretDamage = 50.0f;
	m_turretHealth = 100.0f;
	m_turretBuildCost = 100.0f;
	m_turretShootEffect = ProjectileShootEffect::Basic;
	m_turretFlightEffect = ProjectileFlightEffect::Basic;
	m_turretDestroyEffect = ProjectileDestroyEffect::Count;

	m_abilityDescs = { 
		{PlayerAbility::Heal,	PlayerAbilityDesc(50.0f, "Heal",	Color::Red(),	200.0f)}, 
		{PlayerAbility::Slow,	PlayerAbilityDesc(100.0f, "Slow",	Color::Green(),	150.0f)},
		{PlayerAbility::Damage, PlayerAbilityDesc(75.0f, "Damage",	Color::Brown(),	100.0f)}
	};

	m_upgradeDescs = {
		{PlayerUpgrade::RadiusUp,	PlayerUpgradeDesc(500.0f, "x2 Turret Radius")},
		{PlayerUpgrade::FireRateUp, PlayerUpgradeDesc(500.0f, "x2 Turret Fire Rate")},
		{PlayerUpgrade::DamageUp,	PlayerUpgradeDesc(500.0f, "x2 Turret Damage")},
		{PlayerUpgrade::HealthUp,	PlayerUpgradeDesc(500.0f, "x2 Turret Health")},
		{PlayerUpgrade::CostDown,	PlayerUpgradeDesc(500.0f, "1/2 Turret Build Cost")},
		{PlayerUpgrade::TriShot,	PlayerUpgradeDesc(500.0f, "Tri-Shot")},
		{PlayerUpgrade::Homing,		PlayerUpgradeDesc(500.0f, "Bullet Homing")},
		{PlayerUpgrade::Splash,		PlayerUpgradeDesc(500.0f, "Bullet Splash Damage")}
	};
}

SGame::~SGame()
{
	delete m_waveSpawner;
	delete m_projectileFactory;
}



//--- Scene Interface ---//
void SGame::Init()
{
	LoadLevel();
	CreateQuitToMenuButton();
	SetupPlayer();
	CreateAbilityUI();
	CreateUpgradeUI();
	
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

	// Setup the projectile factory
	m_projectileFactory = new ProjectileFactory(m_registry, std::bind(&SGame::DamageWithBullet, this, P_ARG::_1, P_ARG::_2, P_ARG::_3));
	CShooter::SetProjectileFactory(m_projectileFactory);
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

	// Update the ability
	if (m_abilityReticle != nullptr)
	{
		if (App::IsKeyPressed(/*VK_LBUTTON*/ VK_SPACE))
			TriggerAbility();
		else if (App::IsKeyPressed(VK_RBUTTON))
			CancelAbility();
	}

	// Update the wave spawner
	m_waveSpawner->Update(_deltaTime);

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

		m_turretPlacements[towerBuildLoc] = nullptr;
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

void SGame::DamageWithBullet(CBoxCollider* _a, CBoxCollider* _b, Vec2& _overlap)
{
	Entity* entityA = _a->GetEntity();
	Entity* entityB = _b->GetEntity();

	if ((entityA->GetTag() == EntityTag::Enemy && entityB->GetTag() == EntityTag::BulletPlayer) ||
		(entityA->GetTag() == EntityTag::BulletPlayer && entityB->GetTag() == EntityTag::Enemy))
	{
		// A player bullet has hit an enemy. Figure out which is which
		Entity* enemy = (entityA->GetTag() == EntityTag::Enemy) ? entityA : entityB;
		Entity* playerBullet = (enemy == entityA) ? entityB : entityA;

		// Damage the enemy
		float playerBulletDmg = playerBullet->GetComponent<CProjectile>()->GetDamage();
		enemy->GetComponent<CHealth>()->Damage(playerBulletDmg);

		// If the bullet has splash damage, we should also trigger that
		auto splashDmg = playerBullet->GetComponent<CRadialDamager>();
		if (splashDmg)
			splashDmg->TriggerDamage(playerBullet);

		// Delete the bullet since it collided
		m_registry.DeleteEntity(playerBullet);
	}
	else if ((entityA->GetTag() == EntityTag::Turret && entityB->GetTag() == EntityTag::BulletEnemy) ||
		(entityA->GetTag() == EntityTag::BulletEnemy && entityB->GetTag() == EntityTag::Turret))
	{
		// An enemy bullet has hit a player turret. Figure out which is which
		Entity* turret = (entityA->GetTag() == EntityTag::Turret) ? entityA : entityB;
		Entity* enemyBullet = (turret == entityA) ? entityB : entityA;
		
		// Damage the player turret
		float enemyBulletDmg = enemyBullet->GetComponent<CProjectile>()->GetDamage();
		turret->GetComponent<CHealth>()->Damage(enemyBulletDmg);

		// Delete the bullet since it collided
		m_registry.DeleteEntity(enemyBullet);
	}
}

void SGame::KillEnemy(Entity* _enemy)
{
	// Give the player the correct amount of money
	CEnemy* enemyComp = _enemy->GetComponent<CEnemy>();
	m_playerBank->AddMoney(enemyComp->GetRewardAmount());
}

void SGame::PlaceTurret(Entity* _callingButton)
{
	if (!m_abilityReticle && m_playerBank->GetMoney() >= m_turretBuildCost)
	{
		// Charge the player the cost to build it
		m_playerBank->RemoveMoney(m_turretBuildCost);

		// Build a tower on top of the build location
		{
			auto turret = m_registry.CreateEntity("Turret", EntityTag::Turret);

			CTransform* transformComp = m_registry.AddComponent<CTransform>(turret);
			transformComp->SetPosition(_callingButton->GetComponent<CTransform>()->GetPosition());
			transformComp->Init();

			CSprite* spriteComp = m_registry.AddComponent<CSprite>(turret);
			spriteComp->LoadSprite(".\\GameData\\Sprites\\Turret.bmp");
			spriteComp->SetRenderLayer(-1.0f);
			spriteComp->Init();

			CRadiusIndicator* radiusIndicatorComp = m_registry.AddComponent<CRadiusIndicator>(turret);
			radiusIndicatorComp->SetColor(Color::White(0.2f));
			radiusIndicatorComp->SetRenderLayer(-0.5f);
			radiusIndicatorComp->Init();

			CRadialAimer* aimerComp = m_registry.AddComponent<CRadialAimer>(turret);
			aimerComp->SetRadius(m_turretRadius);
			aimerComp->SetTargetEntityTag(EntityTag::Enemy);
			aimerComp->Init();

			CShooter* shooterComp = m_registry.AddComponent<CShooter>(turret);
			shooterComp->SetFireRate(m_turretFireRate);
			shooterComp->SetBulletTag(EntityTag::BulletPlayer);
			shooterComp->SetShootEffect(m_turretShootEffect);
			shooterComp->SetFlightEffect(m_turretFlightEffect);
			shooterComp->SetDestroyEffect(m_turretDestroyEffect);
			shooterComp->SetBulletDmg(m_turretDamage);
			shooterComp->Init();

			CHealth* healthComp = m_registry.AddComponent<CHealth>(turret);
			healthComp->SetMaxHealth(m_turretHealth);
			healthComp->AddOnDestroyCallback(std::bind(&SGame::OnTurretDestroyed, this, P_ARG::_1));
			healthComp->Init();

			CBoxCollider* boxColliderComp = m_registry.AddComponent<CBoxCollider>(turret);
			boxColliderComp->SetBaseDimensions(32.0f, 32.0f);
			boxColliderComp->Init();

			// Store the turret in the mapping
			m_turretPlacements[_callingButton] = turret;
		}

		// Disable the button now that the tower has been placed
		_callingButton->GetComponent<CButton>()->SetActive(false);
		_callingButton->GetComponent<CLabel>()->SetActive(false);
	}
}

void SGame::OnTurretDestroyed(Entity* _turret)
{
	// Find the corresponding placement button and reset it so the player can make another turret in its place
	for (auto turretPlacement : m_turretPlacements)
	{
		if (turretPlacement.second == _turret)
		{
			// Reset the tile
			Entity* turretTile = turretPlacement.first;
			turretTile->GetComponent<CButton>()->SetActive(true);
			turretTile->GetComponent<CLabel>()->SetActive(true);

			// Clear the turret from the list since it no longer exists
			turretPlacement.second = nullptr;
			return;
		}
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

void SGame::CreateAbilityUI()
{
	Vec2 firstButtonLocation = Vec2(150.0f, 500.0f);
	Vec2 individualOffset = Vec2(0.0f, 60.0f);

	// Create the label
	{
		auto abilityLabel = m_registry.CreateEntity("AbilityLabel");

		CTransform* transformComp = m_registry.AddComponent<CTransform>(abilityLabel);
		transformComp->SetPosition(firstButtonLocation + individualOffset);
		transformComp->Init();

		CLabel* labelComp = m_registry.AddComponent<CLabel>(abilityLabel);
		labelComp->SetText("REUSABLE ABILITIES");
		labelComp->SetOffset(Vec2(-100.0f, 0.0f));
		labelComp->Init();
	}

	// Create the ability buttons
	for (int i = 0; i < (int)PlayerAbility::Count; i++)
	{
		PlayerAbility ability = (PlayerAbility)i;

		auto abilityButton = m_registry.CreateEntity(std::to_string(i));

		CTransform* transformComp = m_registry.AddComponent<CTransform>(abilityButton);
		Vec2 buttonPos = firstButtonLocation - (individualOffset * (float)i);
		transformComp->SetPosition(buttonPos);

		CButton* buttonComp = m_registry.AddComponent<CButton>(abilityButton);
		buttonComp->SetDimensions(Vec2(200.0f, 50.0f));
		buttonComp->SetColor(m_abilityDescs[ability].m_color);
		buttonComp->AddOnClickedCallback(std::bind(&SGame::CreateAbilityReticle, this, P_ARG::_1));

		CLabel* labelComp = m_registry.AddComponent<CLabel>(abilityButton);
		int costAsInt = (int)rint(m_abilityDescs[ability].m_cost);
		labelComp->SetText(m_abilityDescs[ability].m_name + " (" + std::to_string(costAsInt) + ")");
		labelComp->SetColor(m_abilityDescs[ability].m_color);
		labelComp->SetOffset(Vec2(-40.0f, -5.0f));
		labelComp->SetFont(Font::HELVETICA_10);
	}
}

void SGame::CreateAbilityReticle(Entity* _callingButton)
{
	if (!m_abilityReticle)
	{
		// Figure out the ability that was called based on the name of the button
		int buttonId = std::stoi(_callingButton->GetName());
		m_currentAbility = (PlayerAbility)buttonId;

		// If the player has enough money to trigger the ability, go ahead and spawn the reticle
		if (m_playerBank->GetMoney() >= m_abilityDescs[m_currentAbility].m_cost)
		{
			m_playerBank->RemoveMoney(m_abilityDescs[m_currentAbility].m_cost);

			m_abilityReticle = m_registry.CreateEntity("Reticle");

			CTransform* transformComp = m_registry.AddComponent<CTransform>(m_abilityReticle);
			transformComp->Init();

			CMouseFollower* mouseFollowerComp = m_registry.AddComponent<CMouseFollower>(m_abilityReticle);
			mouseFollowerComp->Init();

			CRadiusIndicator* radiusIndicatorComp = m_registry.AddComponent<CRadiusIndicator>(m_abilityReticle);
			radiusIndicatorComp->SetColor(m_abilityDescs[m_currentAbility].m_color);
			radiusIndicatorComp->SetRadius(m_abilityDescs[m_currentAbility].m_radius);
			radiusIndicatorComp->Init();
		}
	}	
}

void SGame::TriggerAbility()
{
	Vec2 abilityCenter = m_abilityReticle->GetComponent<CTransform>()->GetPosition();
	float abilityRadius = m_abilityDescs[m_currentAbility].m_radius;

	switch (m_currentAbility)
	{
		case PlayerAbility::Heal:
		{
			auto turretsInRange = EntityRadiusChecker::GetTaggedComponentsInRange<CHealth>(&m_registry, EntityTag::Turret, abilityCenter, abilityRadius);
			
			for (auto turret : turretsInRange)
				turret->Heal(50.0f);

			break;
		}

		case PlayerAbility::Damage:
		{
			auto enemiesInRange = EntityRadiusChecker::GetTaggedComponentsInRange<CHealth>(&m_registry, EntityTag::Enemy, abilityCenter, abilityRadius);

			for (auto enemy : enemiesInRange)
				enemy->Damage(50.0f);

			break;
		}

		case PlayerAbility::Slow:
		{
			// Place a slow field onto the ground that lasts for 3 seconds
			auto slowField = m_registry.CreateEntity("SlowField");

			CTransform* transformComp = m_registry.AddComponent<CTransform>(slowField);
			transformComp->SetPosition(m_abilityReticle->GetComponent<CTransform>()->GetPosition());
			transformComp->Init();

			CRadiusIndicator* radiusIndicatorComp = m_registry.AddComponent<CRadiusIndicator>(slowField);
			radiusIndicatorComp->SetColor(m_abilityDescs[m_currentAbility].m_color);
			radiusIndicatorComp->Init();

			CRadialSpeeder* radialSpeedComp = m_registry.AddComponent<CRadialSpeeder>(slowField);
			radialSpeedComp->SetRadius(m_abilityDescs[m_currentAbility].m_radius);
			radialSpeedComp->SetSpeedModifier(-0.5f);
			radialSpeedComp->SetTargetEntityTag(EntityTag::Enemy);
			radialSpeedComp->Init();

			CLifetime* lifetimeComp = m_registry.AddComponent<CLifetime>(slowField);
			lifetimeComp->SetMaxLifetime(3.0f);
			lifetimeComp->Init();
		}

		default:
			break;
	}

	// No ability equipped now
	m_registry.DeleteEntity(m_abilityReticle);
	m_abilityReticle = nullptr;
	m_currentAbility = PlayerAbility::None;
}

void SGame::CancelAbility()
{
	// Refund the player their money
	m_playerBank->AddMoney(m_abilityDescs[m_currentAbility].m_cost);

	// No ability equipped now
	m_registry.DeleteEntity(m_abilityReticle);
	m_abilityReticle = nullptr;
	m_currentAbility = PlayerAbility::None;
}

void SGame::CreateUpgradeUI()
{
	Vec2 firstButtonLocation = Vec2(900.0f, 500.0f);
	Vec2 individualOffset = Vec2(0.0f, 60.0f);

	// Create the label
	{
		auto upgradeLabel = m_registry.CreateEntity("UpgradeLabel");

		CTransform* transformComp = m_registry.AddComponent<CTransform>(upgradeLabel);
		transformComp->SetPosition(firstButtonLocation + individualOffset);
		transformComp->Init();

		CLabel* labelComp = m_registry.AddComponent<CLabel>(upgradeLabel);
		labelComp->SetText("ONE-TIME UPGRADES");
		labelComp->SetOffset(Vec2(-100.0f, 0.0f));
		labelComp->Init();
	}

	// Create the upgrade buttons
	for (int i = 0; i < (int)PlayerUpgrade::Count; i++)
	{
		PlayerUpgrade upgrade = (PlayerUpgrade)i;

		// Place the quit to menu button
		auto upgradeButton = m_registry.CreateEntity(std::to_string(i));

		CTransform* transformComp = m_registry.AddComponent<CTransform>(upgradeButton);
		Vec2 buttonPos = firstButtonLocation - (individualOffset * (float)i);
		transformComp->SetPosition(buttonPos);

		CButton* buttonComp = m_registry.AddComponent<CButton>(upgradeButton);
		buttonComp->SetDimensions(Vec2(200.0f, 50.0f));
		buttonComp->SetColor(Color::Green());
		buttonComp->AddOnClickedCallback(std::bind(&SGame::PurchaseUpgrade, this, P_ARG::_1));

		CLabel* labelComp = m_registry.AddComponent<CLabel>(upgradeButton);
		int costAsInt = (int)rint(m_upgradeDescs[upgrade].m_cost);
		labelComp->SetText(m_upgradeDescs[upgrade].m_name + " (" + std::to_string(costAsInt) + ")");
		labelComp->SetColor(Color::Green());
		labelComp->SetOffset(Vec2(-40.0f, -5.0f));
		labelComp->SetFont(Font::HELVETICA_10);
	}
}

void SGame::PurchaseUpgrade(Entity* _callingButton)
{
	m_turretRadius = 100.0f;
	m_turretFireRate = 0.5f;
	m_turretDamage = 50.0f;
	m_turretHealth = 100.0f;
	m_turretBuildCost = 100.0f;
	m_turretShootEffect = ProjectileShootEffect::Basic;
	m_turretFlightEffect = ProjectileFlightEffect::Basic;
	m_turretDestroyEffect = ProjectileDestroyEffect::Count;

	// Figure out the upgrade that was purchased based on the name of the button
	int buttonId = std::stoi(_callingButton->GetName());
	PlayerUpgrade upgrade = (PlayerUpgrade)buttonId;

	// Try to buy the upgrade
	if (m_playerBank->GetMoney() >= m_upgradeDescs[upgrade].m_cost)
	{
		// Charge the player
		m_playerBank->RemoveMoney(m_upgradeDescs[upgrade].m_cost);

		// Perform the upgrade
		switch (upgrade)
		{
			case PlayerUpgrade::RadiusUp:
			{
				auto turretAimers = m_registry.GetAllComponentsByTypeAndTags<CRadialAimer>({ EntityTag::Turret });

				// Double the turret radii
				m_turretRadius *= 2.0f;
				for (auto aimer : turretAimers)
					aimer->SetRadius(m_turretRadius);
				
				break;
			}

			case PlayerUpgrade::FireRateUp:
			{
				auto turretShooters = m_registry.GetAllComponentsByTypeAndTags<CShooter>({ EntityTag::Turret });

				// Half the fire rate (shoots twice as fast)
				m_turretFireRate *= 0.5f;
				for (auto shooter : turretShooters)
					shooter->SetFireRate(m_turretFireRate);

				break;
			}

			case PlayerUpgrade::DamageUp:
			{
				auto turretShooters = m_registry.GetAllComponentsByTypeAndTags<CShooter>({ EntityTag::Turret });

				// Double the damage
				m_turretDamage *= 2.0f;
				for (auto shooter : turretShooters)
					shooter->SetBulletDmg(m_turretDamage);

				break;
			}

			case PlayerUpgrade::HealthUp:
			{
				auto turretHealthComps = m_registry.GetAllComponentsByTypeAndTags<CHealth>({ EntityTag::Turret });

				// Double the health
				m_turretHealth *= 2.0f;
				for (auto healthComp : turretHealthComps)
					healthComp->SetMaxHealth(m_turretHealth, false);

				break;
			}
			
			case PlayerUpgrade::CostDown:
			{
				m_turretBuildCost *= 0.5f;
				break;
			}

			case PlayerUpgrade::TriShot:
			{
				auto turretShooters = m_registry.GetAllComponentsByTypeAndTags<CShooter>({ EntityTag::Turret });

				// Make the turret shoot three bullets at a time
				m_turretShootEffect = ProjectileShootEffect::TriShot;
				for (auto shooter : turretShooters)
					shooter->SetShootEffect(ProjectileShootEffect::TriShot);

				break;
			}

			case PlayerUpgrade::Homing:
			{
				auto turretShooters = m_registry.GetAllComponentsByTypeAndTags<CShooter>({ EntityTag::Turret });

				// Make the projectiles home towards their targets
				m_turretFlightEffect = ProjectileFlightEffect::Homing;
				for (auto shooter : turretShooters)
					shooter->SetFlightEffect(ProjectileFlightEffect::Homing);

				break;
			}

			case PlayerUpgrade::Splash:
			{
				auto turretShooters = m_registry.GetAllComponentsByTypeAndTags<CShooter>({ EntityTag::Turret });

				// Make the projectiles cause splash damage when they hit their targets
				m_turretDestroyEffect = ProjectileDestroyEffect::Splash;
				for (auto shooter : turretShooters)
					shooter->SetDestroyEffect(ProjectileDestroyEffect::Splash);

				break;
			}

			default:
				break;
		}

		// Disable the button to prevent the upgrade from being used again
		_callingButton->GetComponent<CButton>()->SetInteractable(false);
		_callingButton->GetComponent<CButton>()->SetColor(Color::Red());
		_callingButton->GetComponent<CLabel>()->SetColor(Color::Red());
	}
}


//--- Setters ---//
void SGame::SetLevelName(std::string _levelName)
{
	m_levelName = _levelName;
}