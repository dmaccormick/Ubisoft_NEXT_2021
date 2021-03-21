#pragma once

// Standard includes
#include <functional>

// Internal includes
#include "ProjectileTypes.h"
#include "Registry.h"
#include "Vec2.h"
#include "CTransform.h"
#include "CBoxCollider.h"

class ProjectileFactory
{
public:
	//--- Constructors and Destructor ---//
	ProjectileFactory(Registry& _levelRegistry, std::function<void(CBoxCollider*, CBoxCollider*, Vec2&)> _gameCollisionCallback);
	~ProjectileFactory();

	//--- Enemy Prefab Methods ---//
	Entity* CreateProjectile(EntityTag _bulletTag, CTransform* _turret, CTransform* _target, float _damage, ProjectileFlightEffect _flightEffect, ProjectileDestroyEffect _destroyEffect);

private:
	//--- Private Data ---//
	Registry* m_levelRegistry;
	std::function<void(CBoxCollider*, CBoxCollider*, Vec2&)> m_gameCollisionCallback;
};
