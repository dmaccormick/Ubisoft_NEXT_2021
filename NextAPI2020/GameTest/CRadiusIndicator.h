#pragma once

// Internal includes
#include "Component.h"
#include "CSprite.h"
#include "Color.h"

class CRadiusIndicator : public Component
{
	// In the radius image, the circle is exactly 100 pixels in diameter
	// So we can scale the sprite representing it in-game accordingly to accuarately match the radius data
#define INDICATOR_IMAGE_DIAMETER 100.0f

public:
	//--- Constructors and Destructor ---//
	CRadiusIndicator();
	virtual ~CRadiusIndicator();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Methods ---//
	void UpdateSpriteData();

	//--- Setters ---//
	void SetRadius(float _radius);
	void SetColor(Color _color);
	void SetRenderLayer(float _renderLayer);

private:
	//--- Private Data ---//
	Entity* m_radiusEntity;
	CTransform* m_baseEntityTransform;
	CTransform* m_radiusTransform;
	CSprite* m_radiusSprite;
	float m_spriteRenderLayer;
	Color m_color;
	float m_radius;

	//--- Utility Functions ---//
	float CalculateSpriteScale() const;
};
