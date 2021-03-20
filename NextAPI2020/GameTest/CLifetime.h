#pragma once

// Standard includes
#include <functional>

// Internal includes
#include "Component.h"

class CLifetime : public Component
{
public:
	//--- Constructors and Destructor ---//
	CLifetime();
	virtual ~CLifetime();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Methods ---//
	void AddOnDestroyCallback(std::function<void(Entity*)> _onDestroyCallback);
	void RemoveOnDestroyCallback(std::function<void(Entity*)> _onDestroyCallback);
	void Destroy();

	//--- Setters ---//
	void SetMaxLifetime(float _maxLifetime);
	void SetLifetimeLeft(float _lifetimeLeft);

	//--- Getters ---//
	float GetMaxLifetime() const;
	float GetLifetimeLeft() const;

private:
	//--- Private Data ---//
	std::vector<std::function<void(Entity*)>> m_onDestroyCallbacks;
	float m_maxLifetime;
	float m_lifetimeLeft;
};
