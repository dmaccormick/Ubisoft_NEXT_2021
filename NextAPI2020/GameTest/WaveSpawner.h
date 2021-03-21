#pragma once

// Standard includes
#include <vector>
#include <string>
#include <functional>

// Internal includes
#include "Registry.h"
#include "CTransform.h"

struct SubWave
{
	SubWave()
	{
		m_delay = 0.0f;
		m_enemyPrefabFunc = nullptr;
	}

	SubWave(float _delay, std::function<Entity*()> _enemyPrefabFunc)
	{
		m_delay = _delay;
		m_enemyPrefabFunc = _enemyPrefabFunc;
	}

	float m_delay;
	std::function<Entity*()> m_enemyPrefabFunc;
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
	WaveSpawner() {}
	WaveSpawner(Registry& _levelRegistry, CTransform* _enemySpawnerLoc, std::function<void(Entity*)> _onKillCallback);
	~WaveSpawner();

	//--- Methods ---//
	void Update(float _deltaTime);
	void StartNextWave();
	void OnEnemyDestroy(Entity* _enemy);

	//--- Enemy Prefab Functions ---//
	Entity* CreateBasicEnemy();

	//--- Getters ---//
	int GetCurrentWaveNumber() const;
	int GetNumWaves() const;
	int GetEnemyCountRemaining() const;
	bool GetAllWavesComplete() const;

private:
	//--- Private Data ---//
	Registry* m_levelRegistry;
	CTransform* m_enemySpawnerLoc;
	std::vector<Entity*> m_spawnedEnemies;
	std::vector<Wave> m_waves;
	int m_currentWaveNumber;
	int m_currentSubwaveNumber;
	bool m_stillSpawning;
	float m_timeSinceLastSpawn;
	bool m_allWavesComplete;
	std::function<void(Entity*)> m_enemyOnKillCallback;
};