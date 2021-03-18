#pragma once

// Standard includes
#include <map>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <typeinfo>
#include <string>
#include <algorithm>

// Internal includes
#include "Entity.h"
#include "Component.h"

// Definitions
//#define COMP_TYPE_LIST std::map<std::type_index, std::vector<Component*>>

class Registry
{
public:
	//--- Constructors and Destructor ---//
	Registry();
	~Registry();

	//--- Methods ---//
	Entity* CreateEntity(std::string _name = "entity");
	bool DeleteEntity(Entity*);
	void InitAll();
	void UpdateAll(float _deltaTime);

	template<typename CompType>
	CompType* AddComponent(Entity* _entity)
	{
		// Create the component and give it to the entity
		CompType* newComp = new CompType();
		newComp->SetRegistry(this);
		_entity->AddComponent(newComp);

		// If a list of this component type already exists, add it there. Otherwise, make a new list for it
		auto typeIndex = std::type_index(typeid(CompType));
		if (m_allComps.find(typeIndex) != m_allComps.end())
			m_allComps[typeIndex].push_back(newComp);
		else
			m_allComps.emplace(typeIndex, std::vector<Component*>{newComp});

		// Return the new component
		return newComp;
	}	

	template<typename CompType>
	std::vector<CompType*> GetAllComponentsByType()
	{
		auto typeIndex = std::type_index(typeid(CompType));
		if (m_allComps.find(typeIndex) != m_allComps.end())
		{
			std::vector<CompType*> convertedComps;
			auto baseCompList = m_allComps[typeIndex];

			for (auto baseComp : baseCompList)
				convertedComps.push_back((CompType*)baseComp);
			
			return convertedComps;
		}
		else
		{
			return std::vector<CompType*>();
		}
	}

	//tenewComp->SetRegistry(this);mplate<typename CompType>
	//CompType* AddComponent(Entity* _entity)
	//{
	//	// Check if the entity exists
	//	auto entityRef = m_data.find(_entity);
	//	if (entityRef != m_data.end())
	//	{
	//		// Create a new component of the right type
	//		CompType* newComp = new CompType();
	//		newComp->SetEntity(_entity);
	//		newComp->SetRegistry(this);

	//		// Check if the entity already has a component of the given type
	//		auto componentTypeRef = entityRef->second.find(std::type_index(typeid(CompType)));

	//		// If it does, add the new component to the existing list
	//		// Otherwise, make a new list based on the component type and place it there
	//		if (componentTypeRef != entityRef->second.end())
	//			componentTypeRef->second.push_back(newComp);
	//		else
	//			entityRef->second.emplace(std::type_index(typeid(CompType)), std::vector<Component*>{newComp});

	//		// Push it into the list of all components as well, to make it quick and efficient to iterate through later
	//		m_flatComps.push_back(newComp);

	//		// Return the newly added component so it can be easily interacted with afterwards
	//		return newComp;
	//	}

	//	// If it failed, return null
	//	return nullptr;
	//}

	//template<typename CompType>
	//std::vector<CompType*> GetComponents(Entity* _entity)
	//{
	//	// Check if the entity exists
	//	auto entityRef = m_data.find(_entity);
	//	if (entityRef != m_data.end())
	//	{
	//		// Find the components of the given type and return them, if they exist
	//		for (auto compData = entityRef->second.begin(); compData != entityRef->second.end(); compData++)
	//		{
	//			if (std::type_index(typeid(CompType)) == compData->first)
	//			{
	//				auto compVector = &compData->second;
	//				std::vector<CompType*> convertedVec = std::vector<CompType*>();
	//				for (unsigned int i = 0; i < compVector->size(); i++)
	//					convertedVec.push_back((CompType*)compVector->at(i));
	//				return convertedVec;
	//			}
	//		}
	//	}

	//	// Return an empty vector if nothing was found
	//	return std::vector<CompType*>();
	//}

	//template<typename CompType>
	//CompType* GetComponent(Entity* _entity)
	//{
	//	std::vector<CompType*> comps = GetComponents<CompType>(_entity);
	//	if (comps.size() == 0)
	//		return nullptr;
	//	else
	//		return comps[0];
	//}

	//template<typename CompType>
	//std::vector<CompType*> GetAllComponents()
	//{
	//	auto compTypeIndex = std::type_index(typeid(CompType));
	//	std::vector<CompType*> relevantComps = std::vector<CompType*>();

	//	// Find all of the components across all entities that match
	//	for (auto entityData : m_data)
	//	{
	//		for (auto compData : entityData.second)
	//		{
	//			if (compTypeIndex == compData.first)
	//			{
	//				std::vector<Component*>* compVector = &compData.second;
	//				for (unsigned int i = 0; i < compVector->size(); i++)
	//					relevantComps.push_back((CompType*)compVector->at(i));
	//			}
	//		}
	//	}

	//	return relevantComps;
	//}

private:
	//--- Private Data ---//
	//std::vector<Component*> m_flatComps;
	static unsigned int m_nextUniqueID;
	//std::map<Entity*, COMP_TYPE_LIST> m_data;

	std::vector<Entity*> m_allEntities;
	std::map<std::type_index, std::vector<Component*>> m_allComps;
};