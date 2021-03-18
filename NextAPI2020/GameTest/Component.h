#pragma once

// Internal includes
#include "Registry.h"

class Component
{
public:
	//--- Constructors and Destructor ---//
	Component() { m_entity = nullptr; m_registry = nullptr; }
	virtual ~Component() {};

	//--- Component Interface ---//
	virtual void Init() = 0;
	virtual void Update(float _deltaTime) = 0;

	//--- Setters ---//
	void SetEntity(Entity* _newEntity) { m_entity = _newEntity; }
	void SetRegistry(Registry* _registry) { m_registry = _registry; }

	//--- Getters ---//
	Entity* GetEntity() const { return m_entity; }

protected:
	Entity* m_entity;
	Registry* m_registry;
};
