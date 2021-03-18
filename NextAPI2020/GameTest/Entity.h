#pragma once

// Standard includes
#include <string>

class Entity
{
	friend class Registry;
	friend class Component;

public:
	//--- Constructors and Destructor ---//
	Entity(std::string _name, unsigned int _uniqueID) : m_name(_name), m_uniqueID(_uniqueID) {}
	~Entity() {}

private:
	//--- Private Data ---//
	std::string m_name;
	unsigned int m_uniqueID;
};