#pragma once

// Standard includes
#include <string>

// Internal incldues
#include "Color.h"

enum class PlayerAbility
{
	Heal,
	Damage,
	Slow,

	Count,
	None
};

struct PlayerAbilityDesc
{
	PlayerAbilityDesc()
	{
		m_cost = 0.0f;
		m_name = "";
		m_color = Color::White();
		m_radius = 0.0f;
	}

	PlayerAbilityDesc(float _cost, std::string _name, Color _color, float _radius)
	{
		m_cost = _cost;
		m_name = _name;
		m_color = _color;
		m_radius = _radius;
	}

	float m_cost;
	std::string m_name;
	Color m_color;
	float m_radius;
};
