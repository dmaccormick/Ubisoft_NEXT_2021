// Standard header
#include "stdafx.h"

// Internal includes
#include "WaveSpawner.h"

//--- Constructors and Destructor ---//
WaveSpawner::WaveSpawner()
{
	m_enemySpawnerLoc = nullptr;
	m_currentWaveNumber = -1;
	m_currentSubwaveNumber = -1;
	m_stillSpawning = false;
	m_timeSinceLastSpawn = 0.0f;
	m_allWavesComplete = false;
}

WaveSpawner::WaveSpawner(CTransform* _enemySpawnerLoc, Registry& _levelRegistry, std::function<void(Entity*)> _onKillCallback)
{
	m_enemyFactory = EnemyFactory(_enemySpawnerLoc, _levelRegistry, _onKillCallback, std::bind(&WaveSpawner::OnEnemyDestroy, this, P_ARG::_1));
	m_enemySpawnerLoc = _enemySpawnerLoc;
	m_currentWaveNumber = -1;
	m_currentSubwaveNumber = -1;
	m_stillSpawning = false;
	m_timeSinceLastSpawn = 0.0f;
	m_allWavesComplete = false;

	//// Define the test wave
	//// 1 of every enemy
	//m_waves.push_back(Wave({
	//	SubWave(1.0f, EnemyType::Basic),
	//	SubWave(1.0f, EnemyType::Fast),
	//	SubWave(1.0f, EnemyType::Strong),
	//	SubWave(1.0f, EnemyType::Healer),
	//	SubWave(1.0f, EnemyType::Speeder),
	//	SubWave(1.0f, EnemyType::Shooter),
	//	SubWave(1.0f, EnemyType::EMP),
	//	SubWave(1.0f, EnemyType::Bomber),
	//	SubWave(1.0f, EnemyType::Boss),
	//	}));

	// Define the first wave
	m_waves.push_back(Wave({
		SubWave(1.0f, EnemyType::Basic),
		SubWave(1.0f, EnemyType::Basic),
		SubWave(1.0f, EnemyType::Fast),
		SubWave(1.0f, EnemyType::Basic),
		SubWave(1.0f, EnemyType::Strong),
		SubWave(1.0f, EnemyType::Healer),
		SubWave(1.0f, EnemyType::Basic),
		SubWave(1.0f, EnemyType::Fast),
		SubWave(1.0f, EnemyType::Strong),
		SubWave(1.0f, EnemyType::Fast),
		SubWave(1.0f, EnemyType::Basic)
	}));

	// Define the second wave
	m_waves.push_back(Wave({
		SubWave(0.5f, EnemyType::Speeder),
		SubWave(0.5f, EnemyType::Basic),
		SubWave(0.5f, EnemyType::Strong),
		SubWave(0.5f, EnemyType::Fast),
		SubWave(0.5f, EnemyType::Healer),
		SubWave(0.5f, EnemyType::Basic),
		SubWave(0.5f, EnemyType::Basic),
		SubWave(0.5f, EnemyType::EMP),
		SubWave(0.5f, EnemyType::Bomber),
		SubWave(0.5f, EnemyType::Basic)
	}));

	// Define the third wave
	m_waves.push_back(Wave({
		SubWave(0.5f, EnemyType::Speeder),
		SubWave(0.5f, EnemyType::Shooter),
		SubWave(0.5f, EnemyType::Strong),
		SubWave(0.5f, EnemyType::Fast),
		SubWave(0.5f, EnemyType::Healer),
		SubWave(0.5f, EnemyType::Shooter),
		SubWave(0.5f, EnemyType::Fast),
		SubWave(0.5f, EnemyType::EMP),
		SubWave(0.5f, EnemyType::Bomber),
		SubWave(0.5f, EnemyType::Shooter),
		SubWave(0.5f, EnemyType::Speeder),
		SubWave(0.5f, EnemyType::Basic),
		SubWave(0.5f, EnemyType::Strong),
		SubWave(0.5f, EnemyType::Fast),
		SubWave(0.5f, EnemyType::Healer),
		SubWave(0.5f, EnemyType::Shooter),
		SubWave(0.5f, EnemyType::Basic),
		SubWave(0.5f, EnemyType::EMP),
		SubWave(0.5f, EnemyType::Bomber),
		SubWave(0.5f, EnemyType::Healer)
	}));

	// Define the fourth wave
	m_waves.push_back(Wave({
		SubWave(0.5f, EnemyType::Speeder),
		SubWave(0.5f, EnemyType::Bomber),
		SubWave(0.5f, EnemyType::Strong),
		SubWave(0.5f, EnemyType::Fast),
		SubWave(0.5f, EnemyType::Healer),
		SubWave(0.5f, EnemyType::Shooter),
		SubWave(0.5f, EnemyType::Basic),
		SubWave(0.5f, EnemyType::EMP),
		SubWave(0.5f, EnemyType::Bomber),
		SubWave(0.5f, EnemyType::Shooter),
		SubWave(0.5f, EnemyType::Speeder),
		SubWave(0.5f, EnemyType::Healer),
		SubWave(0.5f, EnemyType::Strong),
		SubWave(0.5f, EnemyType::Fast),
		SubWave(0.5f, EnemyType::Healer),
		SubWave(0.5f, EnemyType::Fast),
		SubWave(0.5f, EnemyType::Basic),
		SubWave(0.5f, EnemyType::EMP),
		SubWave(0.5f, EnemyType::Bomber),
		SubWave(0.5f, EnemyType::Healer)
	}));

	// Define the fifth wave
	m_waves.push_back(Wave({
		SubWave(0.5f, EnemyType::Speeder),
		SubWave(0.5f, EnemyType::Bomber),
		SubWave(0.5f, EnemyType::Strong),
		SubWave(0.5f, EnemyType::Fast),
		SubWave(0.5f, EnemyType::Healer),
		SubWave(0.5f, EnemyType::Shooter),
		SubWave(0.5f, EnemyType::Basic),
		SubWave(0.5f, EnemyType::EMP),
		SubWave(0.5f, EnemyType::Bomber),
		SubWave(0.5f, EnemyType::Shooter),
		SubWave(0.5f, EnemyType::Speeder),
		SubWave(0.5f, EnemyType::Healer),
		SubWave(0.5f, EnemyType::Strong),
		SubWave(0.5f, EnemyType::Fast),
		SubWave(0.5f, EnemyType::Healer),
		SubWave(0.5f, EnemyType::Fast),
		SubWave(0.5f, EnemyType::Basic),
		SubWave(0.5f, EnemyType::EMP),
		SubWave(0.5f, EnemyType::Bomber),
		SubWave(3.0f, EnemyType::Healer),
		SubWave(0.5f, EnemyType::Speeder),
		SubWave(0.5f, EnemyType::Bomber),
		SubWave(0.5f, EnemyType::Strong),
		SubWave(0.5f, EnemyType::Fast),
		SubWave(0.5f, EnemyType::Healer),
		SubWave(0.5f, EnemyType::Shooter),
		SubWave(0.5f, EnemyType::Basic),
		SubWave(0.5f, EnemyType::EMP),
		SubWave(0.5f, EnemyType::Bomber),
		SubWave(0.5f, EnemyType::Shooter),
		SubWave(0.5f, EnemyType::Speeder),
		SubWave(0.5f, EnemyType::Healer),
		SubWave(0.5f, EnemyType::Strong),
		SubWave(0.5f, EnemyType::Fast),
		SubWave(0.5f, EnemyType::Healer),
		SubWave(0.5f, EnemyType::Fast),
		SubWave(0.5f, EnemyType::Basic),
		SubWave(0.5f, EnemyType::EMP),
		SubWave(0.5f, EnemyType::Bomber),
		SubWave(0.5f, EnemyType::Boss)
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
			m_spawnedEnemies.push_back(m_enemyFactory.CreateEnemy(currentSubWave->m_enemyType));
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



//--- Getters ---//
int WaveSpawner::GetCurrentWaveNumber() const
{
	return m_currentWaveNumber + 1;
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
