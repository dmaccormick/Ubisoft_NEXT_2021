// Standard header
#include "stdafx.h"

// Internal includes
#include "CLifetime.h"

//--- Constructors and Destructor ---//
CLifetime::CLifetime()
{
	m_maxLifetime = 0.0f;
	m_lifetimeLeft = 0.0f;
}

CLifetime::~CLifetime()
{
}



//--- Component Interface ---//
void CLifetime::Init()
{
}

void CLifetime::Update(float _deltaTime)
{
	float dtSeconds = _deltaTime / 1000.0f;

	m_lifetimeLeft -= dtSeconds;
	if (m_lifetimeLeft <= 0.0f)
		Destroy();
}



//--- Methods ---//
void CLifetime::AddOnDestroyCallback(std::function<void(Entity*)> _onDestroyCallback)
{
	m_onDestroyCallbacks.push_back(_onDestroyCallback);
}

void CLifetime::RemoveOnDestroyCallback(std::function<void(Entity*)> _onDestroyCallback)
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

void CLifetime::Destroy()
{
	// Trigger all of the callbacks and pass them this entity. Afterwards, destroy the entity
	for (auto callback : m_onDestroyCallbacks)
		callback(this->GetEntity());

	m_registry->DeleteEntity(this->GetEntity());
}



//--- Setters ---//
void CLifetime::SetMaxLifetime(float _maxLifetime)
{
	m_maxLifetime = _maxLifetime;
	m_lifetimeLeft = _maxLifetime;
}

void CLifetime::SetLifetimeLeft(float _lifetimeLeft)
{
	m_lifetimeLeft = _lifetimeLeft;
}



//--- Getters ---//
float CLifetime::GetMaxLifetime() const
{
	return m_maxLifetime;
}

float CLifetime::GetLifetimeLeft() const
{
	return m_lifetimeLeft;
}
