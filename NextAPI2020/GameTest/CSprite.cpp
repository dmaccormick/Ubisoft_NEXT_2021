// Standard header
#include "stdafx.h"

// Internal includes
#include "CSprite.h"
#include "App/app.h"

//--- Constructors and Destructor ---//
CSprite::CSprite()
{
	m_sprite = nullptr;
	m_transform = nullptr;
}

CSprite::~CSprite()
{
	if (m_sprite)
	{
		delete m_sprite;
		m_sprite = nullptr;
	}
}



//--- Component Interface ---//
void CSprite::Init()
{
	m_transform = m_entity->GetComponent<CTransform>();
}

void CSprite::Update(float _deltaTime)
{
	Vec2 pos = m_transform->GetPosition();
	float rot = m_transform->GetRotation();
	float scale = m_transform->GetScale();

	m_sprite->SetPosition(pos.GetX(), pos.GetY());
	m_sprite->SetAngle(rot);
	m_sprite->SetScale(scale);

	m_sprite->Update(_deltaTime);
}



//--- Methods ---//
void CSprite::LoadSprite(std::string _filename, int _numCols, int _numRows, float _renderLayer)
{
	m_sprite = App::CreateSprite(_filename.c_str(), _numCols, _numRows);
}

void CSprite::CreateAnimation(unsigned int _id, float _speed, const std::vector<int>& _frames)
{
	m_sprite->CreateAnimation(_id, _speed, _frames);
}

void CSprite::DrawSprite()
{
	m_sprite->Draw();
}


//--- Setters ---//
void CSprite::SetAnimation(unsigned int _id)
{
	m_sprite->SetAnimation(_id);
}

void CSprite::SetFrame(unsigned int _frameNum)
{
	m_sprite->SetFrame(_frameNum);
}

void CSprite::SetRenderLayer(float _renderLayer)
{
	m_sprite->SetRenderLayer(_renderLayer);
}

void CSprite::SetColor(float _r, float _g, float _b, float _a)
{
	m_sprite->SetColor(_r, _g, _b, _a);
}



//--- Getters ---//
float CSprite::GetRenderLayer() const
{
	return m_sprite->GetRenderLayer();
}

CSimpleSprite* CSprite::GetSprite() const
{
	return m_sprite;
}
