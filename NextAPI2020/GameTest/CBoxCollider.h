#pragma once

// Standard includes
#include <functional>

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

	//--- Methods ---//
	void OnCollision(CBoxCollider* _other, Vec2& _overlap);
	void AddCollisionListener(std::function<void(CBoxCollider*, CBoxCollider*, Vec2&)> _callback);
	void RemoveCollisionListener(std::function<void(CBoxCollider*, CBoxCollider*, Vec2&)> _callback);
	static bool CheckCollision(const CBoxCollider* _a, const CBoxCollider* _b, Vec2& _overlap);
	void DebugDraw(float _r = 1.0f, float _g = 1.0f, float _b = 1.0f) const;

	//--- Setters ---//
	void SetBaseDimensions(float _width, float _height);
	void SetBaseDimensions(Vec2 _dimensions);
	void SetBaseDimensions(CSimpleSprite* _sprite);
	void SetEntityOffset(float _x, float _y);
	void SetEntityOffset(Vec2 _offset);

	//--- Getters ---//
	Vec2 GetMin() const;
	Vec2 GetMax() const;
	void GetMinAndMax(Vec2& _min, Vec2& _max) const;

private:
	//--- Private Data ---//
	CTransform* m_transform;
	Vec2 m_baseDimensions;
	Vec2 m_entityOffset;
	Vec2 m_min;
	Vec2 m_max;
	std::vector<std::function<void(CBoxCollider*, CBoxCollider*, Vec2&)>> m_onCollisionListeners;

	//--- Utility Functions ---//
	void RecalculateMinAndMax(const Vec2& _entityPos);
	static bool CheckCollision1D(const Vec2& _line1, const Vec2& _line2);
};