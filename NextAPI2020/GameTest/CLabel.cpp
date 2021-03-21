// Standard header
#include "stdafx.h"

// Internal includes
#include "CLabel.h"
#include "App/app.h"

//--- Statics ---//
std::map<Font, void*> CLabel::m_fontMapping = { 
	{Font::BASE_9_BY_15,	GLUT_BITMAP_9_BY_15},
	{Font::BASE_8_BY_13,	GLUT_BITMAP_8_BY_13},
	{Font::TIMES_ROMAN_10,	GLUT_BITMAP_TIMES_ROMAN_10},
	{Font::TIMES_ROMAN_24,	GLUT_BITMAP_TIMES_ROMAN_24},
	{Font::HELVETICA_10,	GLUT_BITMAP_HELVETICA_10},
	{Font::HELVETICA_12,	GLUT_BITMAP_HELVETICA_12},
	{Font::HELVETICA_18,	GLUT_BITMAP_HELVETICA_18}
};

//--- Constructors and Destructor ---//
CLabel::CLabel()
{
	m_transform = nullptr;
	m_offset = Vec2::Zero();
	m_text = "";
	m_active = true;
	m_color = Color();
	m_font = Font::HELVETICA_18;
}

CLabel::~CLabel()
{

}



//--- Component Interface ---//
void CLabel::Init()
{
	m_transform = GetComponent<CTransform>();
}

void CLabel::Update(float _deltaTime)
{
}



//--- Methods ---//
void CLabel::Draw() const
{
	if (m_active)
	{
		Vec2 pos = m_transform->GetPosition() + m_offset;

		glDisable(GL_DEPTH_TEST);
		App::Print(pos.GetX(), pos.GetY(), m_text.c_str(), m_color.r, m_color.g, m_color.b, m_fontMapping[m_font]);
		glEnable(GL_DEPTH_TEST);
	}
}



//--- Setters ---//
void CLabel::SetText(std::string _text)
{
	m_text = _text;
}

void CLabel::SetOffset(Vec2 _offset)
{
	m_offset = _offset;
}

void CLabel::SetActive(bool _active)
{
	m_active = _active;
}

void CLabel::SetColor(Color _color)
{
	m_color = _color;
}

void CLabel::SetFont(Font _font)
{
	m_font = _font;
}



//--- Getters ---//
std::string CLabel::GetText() const
{
	return m_text;
}

Vec2 CLabel::GetOffset() const
{
	return m_offset;
}

bool CLabel::GetActive() const
{
	return m_active;
}