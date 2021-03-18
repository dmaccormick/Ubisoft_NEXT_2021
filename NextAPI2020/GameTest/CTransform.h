#pragma once

// Internal includes
#include "Component.h"
#include "Vec2.h"

class CTransform : public Component
{
public:
	//--- Constructors and Destructor ---//
	CTransform();
	virtual ~CTransform();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Setters ---//
	void SetPosition(Vec2 _newPos);
	void SetPosition(float _newX, float _newY);
	void SetRotation(float _newRot);
	void SetScale(float _newScale);

	//--- Getters ---//
	Vec2 GetPosition() const;
	float GetRotation() const;
	float GetScale() const;

private:
	//--- Private Data ---//
	Vec2 m_pos;
	float m_rot;
	float m_scale;
};