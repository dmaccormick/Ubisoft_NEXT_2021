// Standard header
#include "stdafx.h"

// Internal includes
#include "CRadiusIndicator.h"

//--- Constructors and Destructor ---//
CRadiusIndicator::CRadiusIndicator()
{
	m_radiusEntity = nullptr;
	m_baseEntityTransform = nullptr;
	m_radiusTransform = nullptr;
	m_radiusSprite = nullptr;
	m_spriteRenderLayer = 5.0f;
	m_radius = 0.0f;
	m_color = Color::White();
}

CRadiusIndicator::~CRadiusIndicator()
{
	m_registry->DeleteEntity(m_radiusEntity);
}



//--- Component Interface ---//
void CRadiusIndicator::Init()
{
	// Get the main entity's transform
	m_baseEntityTransform = m_entity->GetComponent<CTransform>();

	// Create the radius entity and set it up
	{
		m_radiusEntity = m_registry->CreateEntity("Radius");

		m_radiusTransform = m_registry->AddComponent<CTransform>(m_radiusEntity);
		m_radiusTransform->Init();

		m_radiusSprite = m_registry->AddComponent<CSprite>(m_radiusEntity);
		m_radiusSprite->LoadSprite(".\\GameData\\Sprites\\RadiusIndicator.bmp");
		m_radiusSprite->SetRenderLayer(5.0f);
		m_radiusSprite->Init();

		UpdateSpriteData();
	}
}

void CRadiusIndicator::Update(float _deltaTime)
{
	UpdateSpriteData();
}



//--- Methods ---//
void CRadiusIndicator::UpdateSpriteData()
{
	// Move the radius so it stays under the parent object
	// Adjust its scale so it accurately matches the underlying data
	m_radiusTransform->SetPosition(m_baseEntityTransform->GetPosition());
	m_radiusTransform->SetScale(CalculateSpriteScale());

	// Update the visuals
	m_radiusSprite->SetColor(m_color.r, m_color.g, m_color.b, m_color.a);
	m_radiusSprite->SetRenderLayer(m_spriteRenderLayer);
}



//--- Setters ---//
void CRadiusIndicator::SetRadius(float _radius)
{
	m_radius = _radius;
}

void CRadiusIndicator::SetColor(Color _color)
{
	m_color = _color;
}

void CRadiusIndicator::SetRenderLayer(float _renderLayer)
{
	m_spriteRenderLayer = _renderLayer;
}



//--- Getters ---//
Color CRadiusIndicator::GetColor() const
{
	return m_color;
}



//--- Utility Functions ---//
float CRadiusIndicator::CalculateSpriteScale() const
{
	float imageRadius = INDICATOR_IMAGE_DIAMETER * 0.5f;
	return 2.0f * (m_radius / imageRadius);
}