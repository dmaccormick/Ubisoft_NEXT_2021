#pragma once

// Standard includes
#include <functional>

// Internal includes
#include "Component.h"
#include "CLabel.h"

class CHealth : public Component
{
public:
	//--- Constructors and Destructor ---//
	CHealth();
	virtual ~CHealth();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Methods ---//
	void Damage(float _damageAmount);
	void Heal(float _healAmount);
	void AddOnDestroyCallback(std::function<void(Entity*)> _onDestroyCallback);
	void RemoveOnDestroyCallback(std::function<void(Entity*)> _onDestroyCallback);
	void Destroy();

	//--- Setters ---//
	void SetMaxHealth(float _maxHealth, bool m_fullyHeal = true);
	void SetCurrentHealth(float _currentHealth);

	//--- Getters ---//
	float GetMaxHealth() const;
	int GetMaxHealthRounded() const;
	float GetHealth() const;
	int GetHealthRounded() const;

private:
	//--- Private Data ---//
	std::vector<std::function<void(Entity*)>> m_onDestroyCallbacks;
	CLabel* m_healthLabel;
	float m_maxHealth;
	float m_currentHealth;

	//--- Utility Functions ---//
	std::string MakeHealthString() const;
};