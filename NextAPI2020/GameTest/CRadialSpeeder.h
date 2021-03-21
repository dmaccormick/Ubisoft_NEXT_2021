#pragma once

// Internal includes
#include "Component.h"
#include "CTransform.h"
#include "CRadiusIndicator.h"

class CRadialSpeeder : public Component
{
public:
	//--- Constructors and Destructor ---//
	CRadialSpeeder();
	virtual ~CRadialSpeeder();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Setters ---//
	void SetSpeedModifier(float _modifier);
	void SetTargetEntityTag(EntityTag _targetTag);
	void SetRadius(float _radius);

private:
	//--- Private Data ---//
	CTransform* m_transform;
	CRadiusIndicator* m_radiusIndicator;
	float m_speedModifier;
	float m_radius;
	EntityTag m_targetTag;
};
