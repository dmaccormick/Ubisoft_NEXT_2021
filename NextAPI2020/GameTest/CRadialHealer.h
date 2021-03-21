#pragma once

// Internal includes
#include "Component.h"
#include "CRadiusIndicator.h"
#include "CHealth.h"

class CRadialHealer : public Component
{
public:
	//--- Constructors and Destructor ---//
	CRadialHealer();
	virtual ~CRadialHealer();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Setters ---//
	void SetHealRatePerSecond(float _healRatePerSecond);
	void SetTargetEntityTag(EntityTag _targetTag);
	void SetRadius(float _radius);

private:
	//--- Private Data ---//
	CTransform* m_transform;
	CRadiusIndicator* m_radiusIndicator;
	float m_healRatePerSecond;
	float m_radius;
	EntityTag m_targetTag;

	//--- Utility Functions ---//
	bool IsEntityInRange(CHealth* _healthComp) const;
};
