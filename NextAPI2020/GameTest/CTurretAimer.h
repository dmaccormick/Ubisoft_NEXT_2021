#pragma once

// Standard includes
#include <vector>

// Internal includes
#include "Component.h"
#include "CTransform.h"
#include "CRadiusIndicator.h"

class CTurretAimer : public Component
{
public:
	//--- Constructors and Destructor ---//
	CTurretAimer();
	virtual ~CTurretAimer();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Methods ---//
	void FaceTarget();
	void SelectNewTarget();
	void Deactivate();

	//--- Setters ---//
	static void SetEnemyList(const std::vector<Entity*>& _enemies);
	void SetRange(float _range);

	//--- Getters ---//
	CTransform* GetTarget() const;
	float GetRange() const;

private:
	//--- Private Data ---//
	static const std::vector<Entity*>* m_enemies;
	CTransform* m_transform;
	CRadiusIndicator* m_radiusIndicator;
	CTransform* m_currentTarget;
	float m_range;
	bool m_deactivated;
};