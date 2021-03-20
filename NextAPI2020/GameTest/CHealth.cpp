// Standard header
#include "stdafx.h"

// Internal includes
#include "CHealth.h"

//--- Constructors and Destructor ---//
CHealth::CHealth()
{
	m_currentHealth = 0.0f;
	m_maxHealth = 0.0f;
}

CHealth::~CHealth()
{
}



//--- Component Interface ---//
void CHealth::Init()
{
}

void CHealth::Update(float _deltaTime)
{
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
void CHealth::SetMaxHealth(float _maxHealth)
{
	m_maxHealth = _maxHealth;
	m_currentHealth = m_maxHealth;
}

void CHealth::SetCurrentHealth(float _currentHealth)
{
	m_currentHealth = _currentHealth;
}



//--- Getters ---//
float CHealth::GetCurrentHealth() const
{
	return m_currentHealth;
}
