// Standard header
#include "stdafx.h"

// Internal includes
#include "Registry.h"

//--- Static Data ---//
unsigned int Registry::m_nextUniqueID = 0;



//--- Constructors and Destructor ---//
Registry::Registry()
{
	m_allComps = std::vector<Component*>();
	m_data = std::map<Entity*, COMP_TYPE_LIST>();
}

Registry::~Registry()
{
	while (m_data.size() > 0)
		DeleteEntity(m_data.begin()->first);
}

Entity* Registry::CreateEntity(std::string _name)
{
	// Create a new entity and assign it a unique ID
	Entity* newEntity = new Entity(_name, m_nextUniqueID++);

	// Add the entity to the map
	m_data.emplace(std::pair<Entity*, COMP_TYPE_LIST>(newEntity, COMP_TYPE_LIST()));

	// Return the entity
	return newEntity;
}

bool Registry::DeleteEntity(Entity* _entity)
{
	// Check if the entity is in the map
	auto entityRef = m_data.find(_entity);
	if (entityRef != m_data.end())
	{
		// Delete the entity 
		delete entityRef->first;

		// Delete all of the components associated with the entity
		for (auto compData = entityRef->second.begin(); compData != entityRef->second.end(); compData++)
		{
			auto compList = compData->second;

			for (auto comp : compList)
			{
				m_allComps.erase(std::remove(m_allComps.begin(), m_allComps.end(), comp), m_allComps.end());
				delete comp;
			}
		}
		
		// Erase the data from the list
		m_data.erase(_entity);

		// Return true since it worked
		return true;
	}

	// Return false since it failed
	return false;
}

void Registry::InitAll()
{
	for (auto comp : m_allComps)
		comp->Init();
}

void Registry::UpdateAll(float _deltaTime)
{
	for (auto comp : m_allComps)
		comp->Update(_deltaTime);
}