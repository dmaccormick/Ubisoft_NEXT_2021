#pragma once

// Internal includes
#include "Component.h"
#include "CTransform.h"
#include "CLinearMover.h"

class CHomingProjectile : public Component
{
public:
	//--- Constructors and Destructor ---//
	CHomingProjectile();
	virtual ~CHomingProjectile();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Methods ---//
	void ClearTarget(Entity* _target);

	//--- Setters ---//
	void SetTarget(CTransform* _target);

private:
	//--- Private Data ---//
	CTransform* m_transform;
	CLinearMover* m_linearMover;
	CTransform* m_target;
	bool m_shouldHome;
	bool m_homingDelayFinished;
	float m_homingDelay;
	float m_timeSinceStartHoming;
};
