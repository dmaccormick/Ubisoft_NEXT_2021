// Standard header
#include "stdafx.h"

// Internal includes
#include "Entity.h"
#include "Component.h"

//--- Constructors and Destructor ---//
Entity::Entity(std::string _name, unsigned int _uniqueID, EntityTag _tag)
	: m_name(_name), m_uniqueID(_uniqueID), m_tag(_tag)
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

EntityTag Entity::GetTag() const
{
	return m_tag;
}


//--- Private Methods ---//
void Entity::AddComponent(Component* _comp)
{
	_comp->SetEntity(this);
	m_components.push_back(_comp);
}