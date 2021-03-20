// Standard header
#include "stdafx.h"

// Internal includes
#include "Registry.h"

//--- Static Data ---//
unsigned int Registry::m_nextUniqueID = 0;



//--- Constructors and Destructor ---//
Registry::Registry()
{
	m_allEntities = std::vector<Entity*>();
	m_allComps = std::map<std::type_index, std::vector<Component*>>();
}

Registry::~Registry()
{
	while (m_allEntities.size() > 0)
		DeleteEntity(m_allEntities[0]);
}



//--- Methods ---//
Entity* Registry::CreateEntity(std::string _name, EntityTag _tag)
{
	// Create a new entity and assign it a unique ID
	Entity* newEntity = new Entity(_name, m_nextUniqueID++, _tag);

	m_allEntities.push_back(newEntity);

	// Return the entity
	return newEntity;
}

bool Registry::DeleteEntity(Entity* _entity)
{
	// Find the entity in the registry
	if (std::find(m_allEntities.begin(), m_allEntities.end(), _entity) != m_allEntities.end())
	{
		// Erase all of the components from the registry lists
		std::vector<Component*> attachedComps = _entity->m_components;

		for (auto comp : attachedComps)
		{
			auto compType = std::type_index(typeid(comp));
			auto compList = &m_allComps[compType];

			compList->erase(std::remove(compList->begin(), compList->end(), comp), compList->end());
		}

		// Erase the entity from the registry list
		m_allEntities.erase(std::remove(m_allEntities.begin(), m_allEntities.end(), _entity), m_allEntities.end());

		// Delete the entity. It takes care of deleting the components
		delete _entity;

		// Return true to say that the deletion worked
		return true;
	}

	// Return false if the entity could not be found in the registry since it could not be deleted
	return false;
}

void Registry::InitAll()
{
	for (auto compList : m_allComps)
	{
		for (auto comp : compList.second)
			comp->Init();
	}
}

void Registry::UpdateAll(float _deltaTime)
{
	for (auto compList : m_allComps)
	{
		for (auto comp : compList.second)
			comp->Update(_deltaTime);
	}
}



//--- Getters ---//
std::vector<Entity*> Registry::GetAllEntitiesByTags(std::vector<EntityTag> _tags) const
{
	std::vector<Entity*> filteredEntities;

	std::copy_if(m_allEntities.begin(), m_allEntities.end(), std::back_inserter(filteredEntities), 
		[&](Entity* _ent) {return std::find(_tags.begin(), _tags.end(), _ent->GetTag()) != _tags.end(); });

	return filteredEntities;
}

std::vector<Entity*> Registry::GetAllEntitiesByNames(std::vector<std::string> _names) const
{
	std::vector<Entity*> filteredEntities;

	std::copy_if(m_allEntities.begin(), m_allEntities.end(), std::back_inserter(filteredEntities), 
		[&](Entity* _ent) {return std::find(_names.begin(), _names.end(), _ent->GetName()) != _names.end(); });

	return filteredEntities;
}
