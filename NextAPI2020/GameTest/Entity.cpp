// Standard header
#include "stdafx.h"

// Internal includes
#include "Entity.h"
#include "Component.h"

//--- Constructors and Destructor ---//
Entity::Entity(std::string _name, unsigned int _uniqueID)
	: m_name(_name), m_uniqueID(_uniqueID)
{
	m_components = std::vector<Component*>();
}

Entity::~Entity()
{
	for (auto comp : m_components)
		delete comp;
}



//--- Getters ---//
std::string Entity::GetName() const
{
	return m_name;
}


//--- Private Methods ---//
void Entity::AddComponent(Component* _comp)
{
	_comp->SetEntity(this);
	m_components.push_back(_comp);
}