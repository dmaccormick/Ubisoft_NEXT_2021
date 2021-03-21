#pragma once

// Standard includes

// Internal includes
#include "Registry.h"
#include "Vec2.h"

class EntityRadiusChecker
{
public:
	//--- Methods ---//
	template <typename CompType>
	static std::vector<CompType*> GetTaggedComponentsInRange(Registry* _registry, const EntityTag& _entityTag, const Vec2& _center, const float& _radius)
	{
		std::vector<CompType*> allCompsWithTag = _registry->GetAllComponentsByTypeAndTags<CompType>({ _entityTag });
		std::vector<CompType*> compsInRange;

		std::copy_if(allCompsWithTag.begin(), allCompsWithTag.end(), std::back_inserter(compsInRange),
			[&](CompType* _comp) {return CheckIfInRange<CompType>(_comp, _center, _radius); });

		return compsInRange;
	}

private:
	//--- Utility Methods ---//
	template <typename CompType>
	static bool CheckIfInRange(CompType* _comp, const Vec2& _center, const float& _radius)
	{
		float radiusSquared = _radius * _radius;

		Vec2 entityPos = _comp->GetComponent<CTransform>()->GetPosition();
		Vec2 entityToCenter = _center - entityPos;

		float distSquared = entityToCenter.GetMagnitudeSquared();

		return distSquared <= radiusSquared;
	}
};
