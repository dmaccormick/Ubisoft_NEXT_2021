#pragma once

// Internal includes
#include "Component.h"
#include "CTransform.h"
#include "CSprite.h"
#include "Vec2.h"

class CBoxCollider : public Component
{
public:
	//--- Constructors and Destructor ---//
	CBoxCollider();
	virtual ~CBoxCollider();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Setters ---//
	void SetDimensions(float _width, float _height);
	void SetDimensions(Vec2 _dimensions);
	void SetDimensions(CSimpleSprite* _sprite);
	void SetEntityOffset(float _x, float _y);
	void SetEntityOffset(Vec2 _offset);

	//--- Getters ---//
	Vec2 GetMin() const;
	Vec2 GetMax() const;
	void GetMinAndMax(Vec2& _min, Vec2& _max);

private:
	//--- Private Data ---//
	CTransform* m_transform;
	Vec2 m_dimensions;
	Vec2 m_entityOffset;
	Vec2 m_min;
	Vec2 m_max;

	//--- Utility Functions ---//
	void RecalculateMinAndMax(const Vec2& _entityPos);
};