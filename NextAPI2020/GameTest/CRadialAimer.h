#pragma once

// Standard includes
#include <vector>

// Internal includes
#include "Component.h"
#include "CTransform.h"
#include "CRadiusIndicator.h"

class CRadialAimer : public Component
{
public:
	//--- Constructors and Destructor ---//
	CRadialAimer();
	virtual ~CRadialAimer();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Methods ---//
	void FaceTarget();
	void SelectNewTarget();
	void Deactivate();

	//--- Setters ---//
	void SetRadius(float _range);
	void SetTargetEntityTag(EntityTag _targetTag);

	//--- Getters ---//
	CTransform* GetTarget() const;
	float GetRadius() const;

private:
	//--- Private Data ---//
	CTransform* m_transform;
	CRadiusIndicator* m_radiusIndicator;
	CTransform* m_currentTarget;
	float m_radius;
	bool m_deactivated;
	EntityTag m_targetTag;
};