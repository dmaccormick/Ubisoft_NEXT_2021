#pragma once

// Standard includes
#include <vector>
#include <string>
#include <functional>

// Internal includes
#include "Registry.h"
#include "CTransform.h"
#include "EnemyFactory.h"
#include "EnemyType.h"

struct SubWave
{
	SubWave()
	{
		m_delay = 0.0f;
		m_enemyType = EnemyType::Basic;
	}

	SubWave(float _delay, EnemyType _enemyType)
	{
		m_delay = _delay;
		m_enemyType = _enemyType;
	}

	float m_delay;
	EnemyType m_enemyType;
};

struct Wave
{
	Wave() {}
	Wave(std::vector<SubWave> _subwaves)
	{
		m_subwaves = _subwaves;
	}

	std::vector<SubWave> m_subwaves;
};

class WaveSpawner
{
public:
	//--- Constructors and Destructor ---//
	WaveSpawner();
	WaveSpawner(CTransform* _enemySpawnerLoc, Registry& _levelRegistry, std::function<void(Entity*)> _onKillCallback);
	~WaveSpawner();

	//--- Methods ---//
	void Update(float _deltaTime);
	void StartNextWave();
	void OnEnemyDestroy(Entity* _enemy);

	//--- Getters ---//
	int GetCurrentWaveNumber() const;
	int GetNumWaves() const;
	int GetEnemyCountRemaining() const;
	bool GetAllWavesComplete() const;

private:
	//--- Private Data ---//
	EnemyFactory m_enemyFactory;
	CTransform* m_enemySpawnerLoc;
	std::vector<Entity*> m_spawnedEnemies;
	std::vector<Wave> m_waves;
	int m_currentWaveNumber;
	int m_currentSubwaveNumber;
	bool m_stillSpawning;
	float m_timeSinceLastSpawn;
	bool m_allWavesComplete;
};