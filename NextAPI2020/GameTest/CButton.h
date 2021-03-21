#pragma once

// Standard includes
#include <functional>

// Internal includes
#include "Component.h"
#include "Vec2.h"
#include "CTransform.h"
#include "Color.h"

class CButton : public Component
{
public:
	//--- Constructors and Destructor ---//
	CButton();
	virtual ~CButton();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Methods ---//
	void AddOnClickedCallback(std::function<void(Entity*)> _onClickedCallback);
	void RemoveOnClickedCallback(std::function<void(Entity*)> _onClickedCallback);
	void Click() const;
	void Draw(float _r = 1.0f, float _g = 1.0f, float _b = 1.0f) const;

	//--- Setters ---//
	void SetDimensions(Vec2 _dimensions);
	void SetActive(bool _active);
	void SetColor(Color _color);

	//--- Getters ---//
	Vec2 GetDimensions() const;
	bool GetActive() const;

private:
	//--- Private Data ---//
	std::vector<std::function<void(Entity*)>> m_onClickCallbacks;
	CTransform* m_transform;
	bool m_active;
	Vec2 m_dimensions;
	Vec2 m_min;
	Vec2 m_max;
	Color m_color;

	//--- Utility Functions ---//
	void RecalulateMinAndMax();
};
