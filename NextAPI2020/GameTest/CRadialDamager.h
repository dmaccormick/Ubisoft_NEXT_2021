#pragma once

// Internal includes
#include "Component.h"
#include "CTransform.h"
#include "CRadiusIndicator.h"

class CRadialDamager : public Component
{
public:
	//--- Constructors and Destructor ---//
	CRadialDamager();
	virtual ~CRadialDamager();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Methods ---//
	void TriggerDamage(Entity* _callingEntity);

	//--- Setters ---//
	void SetDamageAmount(float _damageAmount);
	void SetTargetEntityTag(EntityTag _targetTag);
	void SetRadius(float _radius);

private:
	//--- Private Data ---//
	CTransform* m_transform;
	CRadiusIndicator* m_radiusIndicator;
	float m_damageAmount;
	float m_radius;
	EntityTag m_targetTag;
};
