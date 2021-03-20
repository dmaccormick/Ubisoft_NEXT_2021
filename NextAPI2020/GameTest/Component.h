#pragma once

// Internal includes
#include "Registry.h"

class Component
{
	friend class Registry;
	friend class Entity;

public:
	//--- Constructors and Destructor ---//
	Component() { m_entity = nullptr; m_registry = nullptr; }
	virtual ~Component() {};

	//--- Component Interface ---//
	virtual void Init() = 0;
	virtual void Update(float _deltaTime) = 0;

	//--- Getters ---//
	template<typename CompType>
	CompType* GetComponent() const { return m_entity->GetComponent<CompType>(); }
	Entity* GetEntity() const { return m_entity; }

protected:
	//--- Protected Data ---//
	Entity* m_entity;
	Registry* m_registry;

	//--- Private Setters ---//
	void SetEntity(Entity* _newEntity) { m_entity = _newEntity; } // Private so they can only be set by the registry or entities
	void SetRegistry(Registry* _registry) { m_registry = _registry; }
};
