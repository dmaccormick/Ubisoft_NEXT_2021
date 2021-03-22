#pragma once

// Standard includes
#include <string>

enum class PlayerUpgrade
{
	RadiusUp,
	FireRateUp,
	DamageUp,
	HealthUp,
	CostDown,
	TriShot,
	Homing,
	Splash,

	Count
};

struct PlayerUpgradeDesc
{
	PlayerUpgradeDesc()
	{
		m_cost = 0.0f;
		m_name = "";
	}

	PlayerUpgradeDesc(float _cost, std::string _name)
	{
		m_cost = _cost;
		m_name = _name;
	}

	float m_cost;
	std::string m_name;
};