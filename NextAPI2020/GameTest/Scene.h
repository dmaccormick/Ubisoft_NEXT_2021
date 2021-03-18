#pragma once

// Internal includes
#include "Registry.h"

class Scene
{
public:
	//--- Constructors and Destructors ---//
	Scene() {}
	virtual ~Scene() {}

	//--- Methods ---//
	virtual void Init() = 0;
	virtual void Update(float _deltaTime) = 0;
	virtual void Draw() = 0;

protected:
	//--- Protected Data ---//
	Registry m_registry;
};