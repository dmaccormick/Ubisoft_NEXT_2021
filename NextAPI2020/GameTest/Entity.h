#pragma once

// Standard includes
#include <string>
#include <vector>

class Component;

class Entity
{
	friend class Registry;

public:
	//--- Constructors and Destructor ---//
	Entity(std::string _name, unsigned int _uniqueID);
	~Entity();

	//--- Methods ---//
	// Find the first (and/or only) component that matches the requested type
	template<typename CompType> 
	CompType* GetComponent()
	{
		for (auto comp : m_components)
		{
			if (std::type_index(typeid(*comp)) == std::type_index(typeid(CompType)))
				return (CompType*)comp;
		}

		return nullptr;
	}

	// Find all of the components that match the requested type
	template<typename CompType>
	std::vector<CompType*> GetComponents()
	{
		std::vector<CompType*> relevantComps;

		for (auto comp : m_components)
		{
			if (std::type_index(typeid(*comp)) == std::type_index(typeid(CompType)))
				relevantComps.push_back(comp);
		}

		return relevantComps;
	}

private:
	//--- Private Data ---//
	std::string m_name;
	unsigned int m_uniqueID;
	std::vector<Component*> m_components;

	//--- Private Methods ---//
	void AddComponent(Component* _comp);
};