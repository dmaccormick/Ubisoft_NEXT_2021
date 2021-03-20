#pragma once

// Internal includes
#include "Component.h"
#include "Vec2.h"
#include "CTransform.h"

class CPathFollower : public Component
{
public:
	//--- Constructors and Destructor ---//
	CPathFollower();
	virtual ~CPathFollower();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Setters ---//
	void SetMovementDirection(Vec2 _movementDir);
	void SetMovementSpeed(float _moveSpeed);

	//--- Getters ---//
	Vec2 GetMovementDirection() const;
	float GetMovementSpeed() const;

private:
	//--- Private Data ---//
	CTransform* m_transform;
	Vec2 m_movementDir;
	float m_movementSpeed;
};