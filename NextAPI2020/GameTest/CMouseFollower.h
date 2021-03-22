#pragma once

// Internal includes
#include "Component.h"
#include "CTransform.h"

class CMouseFollower : public Component
{
public:
	//--- Constructors and Destructor ---//
	CMouseFollower();
	virtual ~CMouseFollower();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

private:
	//--- Private Data ---//
	CTransform* m_transform;
};
