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

class Registry
{
public:
	//--- Constructors and Destructor ---//
	Registry();
	~Registry();

	//--- Methods ---//
	Entity* CreateEntity(std::string _name = "entity", EntityTag _tag = EntityTag::None);
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



	//--- Getters ---//
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

	template<typename CompType>
	std::vector<CompType*> GetAllComponentsByTypeAndTags(std::vector<EntityTag> _tags)
	{
		std::vector<CompType*> baseCompList = GetAllComponentsByType<CompType>();
		std::vector<CompType*> filteredCompList;

		std::copy_if(baseCompList.begin(), baseCompList.end(), std::back_inserter(filteredCompList), 
			[&](CompType* _comp) {return std::find(_tags.begin(), _tags.end(), _comp->GetEntity()->GetTag()) != _tags.end(); });
		
		return filteredCompList;
	}

	std::vector<Entity*> GetAllEntitiesByTags(std::vector<EntityTag> _tags) const;
	std::vector<Entity*> GetAllEntitiesByNames(std::vector<std::string> _names) const;

private:
	//--- Private Data ---//
	static unsigned int m_nextUniqueID;
	std::vector<Entity*> m_allEntities;
	std::map<std::type_index, std::vector<Component*>> m_allComps;
};