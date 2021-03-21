// Standard header
#include "stdafx.h"

// Internal includes
#include "WaveSpawner.h"
#include "CTransform.h"
#include "CProjectile.h"
#include "CSprite.h"
#include "CBoxCollider.h"
#include "CLifetime.h"
#include "CLinearMover.h"
#include "CHealth.h"
#include "CEnemy.h"

//--- Constructors and Destructor ---//
WaveSpawner::WaveSpawner(Registry& _levelRegistry, CTransform* _enemySpawnerLoc, std::function<void(Entity*)> _onKillCallback)
{
	m_levelRegistry = &_levelRegistry;
	m_enemySpawnerLoc = _enemySpawnerLoc;
	m_enemyOnKillCallback = _onKillCallback;
	m_currentWaveNumber = -1;
	m_currentSubwaveNumber = -1;
	m_stillSpawning = false;
	m_timeSinceLastSpawn = 0.0f;
	m_allWavesComplete = false;

	// Define the first wave
	// 10 basic enemies, spaced out exactly 1 second apart
	m_waves.push_back(Wave({
		SubWave(1.0f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(1.0f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(1.0f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(1.0f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(1.0f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(1.0f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(1.0f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(1.0f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(1.0f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(1.0f, std::bind(&WaveSpawner::CreateBasicEnemy, this))
	}));

	// Define the second wave
	// 10 basic enemies, spaced out 0.5 seconds apart
	m_waves.push_back(Wave({
		SubWave(0.5f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(0.5f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(0.5f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(0.5f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(0.5f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(0.5f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(0.5f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(0.5f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(0.5f, std::bind(&WaveSpawner::CreateBasicEnemy, this)),
		SubWave(0.5f, std::bind(&WaveSpawner::CreateBasicEnemy, this))
		}));
}

WaveSpawner::~WaveSpawner()
{
}



//--- Methods ---//
void WaveSpawner::Update(float _deltaTime)
{
	if (!m_allWavesComplete && m_currentWaveNumber != -1 && m_stillSpawning)
	{
		// If enough time has passed, we should spawn this subwave's enemy and move to the next one
		float dtSeconds = _deltaTime / 1000.0f;
		m_timeSinceLastSpawn += dtSeconds;

		Wave* currentWave = &m_waves[m_currentWaveNumber];
		SubWave* currentSubWave = &currentWave->m_subwaves[m_currentSubwaveNumber];

		if (m_timeSinceLastSpawn > currentSubWave->m_delay)
		{
			m_timeSinceLastSpawn = 0.0f;

			// Spawn the enemy and store it in the list so we can keep track
			m_spawnedEnemies.push_back(currentSubWave->m_enemyPrefabFunc());
			m_currentSubwaveNumber++;

			// If that was the last subwave, end the spawning and now just wait until the wave is cleared by the player
			if (m_currentSubwaveNumber >= currentWave->m_subwaves.size())
				m_stillSpawning = false;
		}
	}
}

void WaveSpawner::StartNextWave()
{
	m_currentWaveNumber++;
	m_currentSubwaveNumber = 0;
	m_stillSpawning = true;
	m_timeSinceLastSpawn = 0.0f;

	if (m_currentWaveNumber >= m_waves.size())
		m_allWavesComplete = true;
}

void WaveSpawner::OnEnemyDestroy(Entity* _enemy)
{
	// Remove the enemy from the list
	m_spawnedEnemies.erase(std::remove(m_spawnedEnemies.begin(), m_spawnedEnemies.end(), _enemy), m_spawnedEnemies.end());

	// If the list is now empty and there are no more enemies to spawn, the wave is complete and we can move on
	if (!m_stillSpawning && m_spawnedEnemies.size() == 0)
		StartNextWave();
}



//--- Enemy Prefab Functions ---//
Entity* WaveSpawner::CreateBasicEnemy()
{
	auto enemy = m_levelRegistry->CreateEntity("BasicEnemy", EntityTag::Enemy);

	CTransform* transformComp = m_levelRegistry->AddComponent<CTransform>(enemy);
	transformComp->SetPosition(m_enemySpawnerLoc->GetPosition());
	transformComp->Init();

	CSprite* spriteComp = m_levelRegistry->AddComponent<CSprite>(enemy);
	spriteComp->LoadSprite(".\\GameData\\Sprites\\Enemy_Base.bmp");
	spriteComp->Init();

	CBoxCollider* boxCollider = m_levelRegistry->AddComponent<CBoxCollider>(enemy);
	boxCollider->SetBaseDimensions(Vec2(5.0f, 5.0f));
	boxCollider->Init();

	CLinearMover* linearMoverComp = m_levelRegistry->AddComponent<CLinearMover>(enemy);
	linearMoverComp->SetMovementSpeed(50.0f);
	linearMoverComp->Init();

	CHealth* healthComp = m_levelRegistry->AddComponent<CHealth>(enemy);
	healthComp->SetMaxHealth(25.0f);
	healthComp->Init();
	healthComp->AddOnDestroyCallback(m_enemyOnKillCallback);
	healthComp->AddOnDestroyCallback(std::bind(&WaveSpawner::OnEnemyDestroy, this, P_ARG::_1));

	CEnemy* enemyComp = m_levelRegistry->AddComponent<CEnemy>(enemy);
	enemyComp->SetDamage(100.0f);
	enemyComp->SetRewardAmount(50.0f);
	enemyComp->Init();

	return enemy;
}



//--- Getters ---//
int WaveSpawner::GetCurrentWaveNumber() const
{
	return m_currentWaveNumber;
}

int WaveSpawner::GetNumWaves() const
{
	return static_cast<int>(m_waves.size());
}

int WaveSpawner::GetEnemyCountRemaining() const
{
	int numSpawnedEnemies = static_cast<int>(m_spawnedEnemies.size());
	int numSubwaves = static_cast<int>(m_waves[m_currentWaveNumber].m_subwaves.size());

	return numSpawnedEnemies + (numSubwaves - m_currentSubwaveNumber);
}

bool WaveSpawner::GetAllWavesComplete() const
{
	return m_allWavesComplete;
}
