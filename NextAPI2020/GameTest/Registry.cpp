// Standard header
#include "stdafx.h"

// Internal includes
#include "Registry.h"

//--- Static Data ---//
unsigned int Registry::m_nextUniqueID = 0;



//--- Constructors and Destructor ---//
Registry::Registry()
{
	//m_flatComps = std::vector<Component*>();
	//m_data = std::map<Entity*, COMP_TYPE_LIST>();
	m_allEntities = std::vector<Entity*>();
	m_allComps = std::map<std::type_index, std::vector<Component*>>();
}

Registry::~Registry()
{
	/*while (m_data.size() > 0)
		DeleteEntity(m_data.begin()->first);*/

	for (auto entity : m_allEntities)
		DeleteEntity(entity);
}

Entity* Registry::CreateEntity(std::string _name)
{
	// Create a new entity and assign it a unique ID
	Entity* newEntity = new Entity(_name, m_nextUniqueID++);

	// Add the entity to the list
	//m_data.emplace(std::pair<Entity*, COMP_TYPE_LIST>(newEntity, COMP_TYPE_LIST()));
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

//bool Registry::DeleteEntity(Entity* _entity)
//{
//	// Check if the entity is in the map
//	auto entityRef = m_data.find(_entity);
//	if (entityRef != m_data.end())
//	{
//		// Delete the entity 
//		delete entityRef->first;
//
//		// Delete all of the components associated with the entity
//		for (auto compData = entityRef->second.begin(); compData != entityRef->second.end(); compData++)
//		{
//			auto compList = compData->second;
//
//			for (auto comp : compList)
//			{
//				m_flatComps.erase(std::remove(m_flatComps.begin(), m_flatComps.end(), comp), m_flatComps.end());
//				delete comp;
//			}
//		}
//		
//		// Erase the data from the list
//		m_data.erase(_entity);
//
//		// Return true since it worked
//		return true;
//	}
//
//	// Return false since it failed
//	return false;
//}

void Registry::InitAll()
{
	/*for (auto comp : m_flatComps)
		comp->Init();*/

	for (auto compList : m_allComps)
	{
		for (auto comp : compList.second)
			comp->Init();
	}
}

void Registry::UpdateAll(float _deltaTime)
{
	/*for (auto comp : m_flatComps)
		comp->Update(_deltaTime);*/

	for (auto compList : m_allComps)
	{
		for (auto comp : compList.second)
			comp->Update(_deltaTime);
	}
}