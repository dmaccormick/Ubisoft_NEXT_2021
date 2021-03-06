#pragma once

// Standard includes
#include <string>

// Internal includes
#include "Scene.h"
#include "CBoxCollider.h"
#include "CRadialAimer.h"
#include "CLabel.h"
#include "CBank.h"
#include "CHealth.h"
#include "SEndScreen.h"
#include "VictoryState.h"
#include "WaveSpawner.h"
#include "ProjectileFactory.h"
#include "PlayerAbility.h"
#include "PlayerUpgrade.h"

class SGame : public Scene
{
public:
	//--- Constructors and Destructor ---//
	SGame();
	~SGame();

	//--- Scene Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;
	virtual void Draw() override;

	//--- Game Logic Methods ---//
	void LoadLevel();
	void SetupPlayer();
	void CreateQuitToMenuButton();
	void CheckCollisions();
	void TriggerEnemyDirectionChange(CBoxCollider* _a, CBoxCollider* _b, Vec2& _overlap);
	void AttackPlayerBase(CBoxCollider* _a, CBoxCollider* _b, Vec2& _overlap);
	void DamageWithBullet(CBoxCollider* _a, CBoxCollider* _b, Vec2& _overlap);
	void KillEnemy(Entity* _entity);
	void PlaceTurret(Entity* _callingButton);
	void OnTurretDestroyed(Entity* _turret);
	void GameOver(Entity* _playerEntity);
	void QuitToMenu(Entity* _callingButton);
	void CreateAbilityUI();
	void CreateAbilityReticle(Entity* _callingButton);
	void TriggerAbility();
	void CancelAbility();
	void CreateUpgradeUI();
	void PurchaseUpgrade(Entity* _callingButton);

	//--- Setters ---//
	static void SetLevelName(std::string _levelName);

private:
	//--- Private Data ---//
	static std::string m_levelName;
	std::vector<Entity*> m_levelPieces;
	float m_timeBetweenEnemies;
	float m_timeSinceLastEnemy;
	std::map<Entity*, Entity*> m_turretPlacements;
	Entity* m_player;
	CBank* m_playerBank;
	CLabel* m_playerMoneyLabel;
	CHealth* m_playerHealth;
	CLabel* m_playerHealthLabel;
	CLabel* m_waveNumberLabel;
	CLabel* m_enemiesLeftLabel;
	VictoryState m_victoryState;
	bool m_quitToMenu;
	WaveSpawner* m_waveSpawner;
	ProjectileFactory* m_projectileFactory;
	Entity* m_abilityReticle;
	PlayerAbility m_currentAbility;
	std::map<PlayerAbility, PlayerAbilityDesc> m_abilityDescs;
	std::map<PlayerUpgrade, PlayerUpgradeDesc> m_upgradeDescs;

	float m_turretRadius;
	float m_turretFireRate;
	float m_turretDamage;
	float m_turretHealth;
	float m_turretBuildCost;
	ProjectileShootEffect m_turretShootEffect;
	ProjectileFlightEffect m_turretFlightEffect;
	ProjectileDestroyEffect m_turretDestroyEffect;
};