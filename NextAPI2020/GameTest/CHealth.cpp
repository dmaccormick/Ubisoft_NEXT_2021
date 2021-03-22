// Standard header
#include "stdafx.h"

// Standard includes
#include <math.h>

// Internal includes
#include "CHealth.h"

//--- Constructors and Destructor ---//
CHealth::CHealth()
{
	m_currentHealth = 0.0f;
	m_maxHealth = 0.0f;
	m_healthLabel = nullptr;
}

CHealth::~CHealth()
{
}



//--- Component Interface ---//
void CHealth::Init()
{
	// Attach the health label which will be used to show the current and max HP of the object
	m_healthLabel = m_registry->AddComponent<CLabel>(m_entity);
	m_healthLabel->SetFont(Font::BASE_8_BY_13);
	m_healthLabel->SetColor(Color::White());
	m_healthLabel->SetOffset(Vec2(-22.5f, 20.0f));
	m_healthLabel->Init();
}

void CHealth::Update(float _deltaTime)
{
	m_healthLabel->SetText(MakeHealthString());
}



//--- Methods ---//
void CHealth::Damage(float _damageAmount)
{
	m_currentHealth -= _damageAmount;

	if (m_currentHealth <= 0.0f)
		Destroy();
}

void CHealth::Heal(float _healAmount)
{
	m_currentHealth += _healAmount;

	// Don't go above the max health
	if (m_currentHealth > m_maxHealth)
		m_currentHealth = m_maxHealth;
}

void CHealth::AddOnDestroyCallback(std::function<void(Entity*)> _onDestroyCallback)
{
	m_onDestroyCallbacks.push_back(_onDestroyCallback);
}

void CHealth::RemoveOnDestroyCallback(std::function<void(Entity*)> _onDestroyCallback)
{
	for (unsigned int i = 0; i < m_onDestroyCallbacks.size(); i++)
	{
		auto listener = m_onDestroyCallbacks[i];

		// Remove the listener if it points to the same target and type since there is no way of directly comparing std::function's
		if (listener.target<void(Entity*)>() == _onDestroyCallback.target<void(Entity*)>())
		{
			if (listener.target_type() == _onDestroyCallback.target_type())
			{
				m_onDestroyCallbacks.erase(m_onDestroyCallbacks.begin() + i);
				break;
			}
		}
	}
}

void CHealth::Destroy()
{
	// Trigger all of the callbacks and pass them this entity. Afterwards, destroy the entity
	for (auto callback : m_onDestroyCallbacks)
		callback(this->GetEntity());

	m_registry->DeleteEntity(this->GetEntity());
}



//--- Setters ---//
void CHealth::SetMaxHealth(float _maxHealth, bool _fullyHeal)
{
	m_maxHealth = _maxHealth;

	if (_fullyHeal)
		m_currentHealth = m_maxHealth;
}

void CHealth::SetCurrentHealth(float _currentHealth)
{
	m_currentHealth = _currentHealth;
}



//--- Getters ---//
float CHealth::GetMaxHealth() const
{
	return m_maxHealth;
}

int CHealth::GetMaxHealthRounded() const
{
	return (int)rint(m_maxHealth);
}

float CHealth::GetHealth() const
{
	return m_currentHealth;
}

int CHealth::GetHealthRounded() const
{
	return (int)rint(m_currentHealth);
}



//--- Utility Methods ---//
std::string CHealth::MakeHealthString() const
{
	return std::to_string(GetHealthRounded()) + "/" + std::to_string(GetMaxHealthRounded());
}
