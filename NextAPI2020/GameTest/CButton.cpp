// Standard header
#include "stdafx.h"

// Internal includes
#include "CButton.h"
#include "App/app.h"

//--- Constructors and Destructor ---//
CButton::CButton()
{
	m_transform = nullptr;
	m_active = true;
	m_interactable = true;
	m_dimensions = Vec2::Zero();
	m_min = Vec2::Zero();
	m_max = Vec2::Zero();
	m_color = Color();
}

CButton::~CButton()
{
}



//--- Component Interface ---//
void CButton::Init()
{
	m_transform = GetComponent<CTransform>();
}

void CButton::Update(float _deltaTime)
{
	// If the button is active, check for a mouse click within the bounds of the button
	// If there is one, trigger the click callbacks
	if (m_active && m_interactable)
	{
		RecalulateMinAndMax();

		if (App::IsKeyPressed(VK_LBUTTON))
		{
			float mouseX, mouseY;
			App::GetMousePos(mouseX, mouseY);

			if (mouseX >= m_min.GetX() && mouseX <= m_max.GetX() &&
				mouseY >= m_min.GetY() && mouseY <= m_max.GetY())
			{
				Click();
			}
		}
	}
}



//--- Methods ---//
void CButton::AddOnClickedCallback(std::function<void(Entity*)> _onClickedCallback)
{
	m_onClickCallbacks.push_back(_onClickedCallback);
}

void CButton::RemoveOnClickedCallback(std::function<void(Entity*)> _onClickedCallback)
{
	for (unsigned int i = 0; i < m_onClickCallbacks.size(); i++)
	{
		auto listener = m_onClickCallbacks[i];

		// Remove the listener if it points to the same target and type since there is no way of directly comparing std::function's
		if (listener.target<void(CButton*)>() == _onClickedCallback.target<void(CButton*)>())
		{
			if (listener.target_type() == _onClickedCallback.target_type())
			{
				m_onClickCallbacks.erase(m_onClickCallbacks.begin() + i);
				break;
			}
		}
	}
}

void CButton::Click() const
{
	for (auto onClick : m_onClickCallbacks)
		onClick(this->GetEntity());
}

void CButton::Draw(float _r, float _g, float _b) const
{
	if (m_active)
	{
		glDisable(GL_DEPTH_TEST);
		App::DrawLine(m_min.GetX(), m_min.GetY(), m_max.GetX(), m_min.GetY(), m_color.r, m_color.g, m_color.b);
		App::DrawLine(m_max.GetX(), m_min.GetY(), m_max.GetX(), m_max.GetY(), m_color.r, m_color.g, m_color.b);
		App::DrawLine(m_max.GetX(), m_max.GetY(), m_min.GetX(), m_max.GetY(), m_color.r, m_color.g, m_color.b);
		App::DrawLine(m_min.GetX(), m_max.GetY(), m_min.GetX(), m_min.GetY(), m_color.r, m_color.g, m_color.b);
		glEnable(GL_DEPTH_TEST);
	}
}



//--- Setters ---//
void CButton::SetDimensions(Vec2 _dimensions)
{
	m_dimensions = _dimensions;
}

void CButton::SetActive(bool _active)
{
	m_active = _active;
}

void CButton::SetColor(Color _color)
{
	m_color = _color;
}

void CButton::SetInteractable(bool _interactable)
{
	m_interactable = _interactable;
}


//--- Getters ---//
Vec2 CButton::GetDimensions() const
{
	return m_dimensions;
}

bool CButton::GetActive() const
{
	return m_active;
}



//--- Utility Functions ---//
void CButton::RecalulateMinAndMax()
{
	Vec2 pos = m_transform->GetPosition();
	Vec2 halfDimensions = m_dimensions * 0.5f * m_transform->GetScale();

	m_min = pos - halfDimensions;
	m_max = pos + halfDimensions;
}