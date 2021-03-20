// Standard header
#include "stdafx.h"

// Internal includes
#include "CBoxCollider.h"
#include "App/app.h"

//--- Constructors and Destructor ---//
CBoxCollider::CBoxCollider()
{
	m_transform = nullptr;
	m_baseDimensions = Vec2(1.0f, 1.0f);
	m_entityOffset = Vec2(0.0f, 0.0f);
	m_min = Vec2(-0.5f, -0.5f);
	m_max = Vec2(0.5f, 0.5f);
}

CBoxCollider::~CBoxCollider()
{
	m_transform = nullptr;
}



//--- Component Interface ---//
void CBoxCollider::Init()
{
	m_transform = GetComponent<CTransform>();
}

void CBoxCollider::Update(float _deltaTime)
{
	const Vec2 newEntityPos = m_transform->GetPosition();

	RecalculateMinAndMax(newEntityPos);
}



//--- Methods ---//
void CBoxCollider::OnCollision(CBoxCollider* _other, Vec2& _overlap)
{
	// Invoke all of the callback functions
	for (auto callback : m_onCollisionListeners)
		callback(this, _other, _overlap);
}

void CBoxCollider::AddCollisionListener(std::function<void(CBoxCollider*, CBoxCollider*, Vec2&)> _callback)
{
	m_onCollisionListeners.push_back(_callback);
}

void CBoxCollider::RemoveCollisionListener(std::function<void(CBoxCollider*, CBoxCollider*, Vec2&)> _callback)
{
	for (unsigned int i = 0; i < m_onCollisionListeners.size(); i++)
	{
		auto listener = m_onCollisionListeners[i];

		// Remove the listener if it points to the same target and type since there is no way of directly comparing std::function's
		if (listener.target<void(CBoxCollider*, CBoxCollider*)>() == _callback.target<void(CBoxCollider*, CBoxCollider*)>())
		{
			if (listener.target_type() == _callback.target_type())
			{
				m_onCollisionListeners.erase(m_onCollisionListeners.begin() + i);
				break;
			}
		}
	}
}

bool CBoxCollider::CheckCollision(const CBoxCollider* _a, const CBoxCollider* _b, Vec2& _overlap)
{
	Vec2 aMin = _a->GetMin(), aMax = _a->GetMax();
	Vec2 bMin = _b->GetMin(), bMax = _b->GetMax();

	Vec2 xAxisA = Vec2(aMin.GetX(), aMax.GetX());
	Vec2 xAxisB = Vec2(bMin.GetX(), bMax.GetX());

	if (CBoxCollider::CheckCollision1D(xAxisA, xAxisB))
	{
		Vec2 yAxisA = Vec2(aMin.GetY(), aMax.GetY());
		Vec2 yAxisB = Vec2(bMin.GetY(), bMax.GetY());

		if (CBoxCollider::CheckCollision1D(yAxisA, yAxisB))
		{
			// If completely within the bounds of the other object, don't need to move at all
			if (aMin.GetX() >= bMin.GetX() && aMax.GetX() <= bMax.GetX())
				_overlap.SetX(0.0f);
			else if (bMin.GetX() >= aMin.GetX() && bMax.GetX() <= aMax.GetX())
				_overlap.SetX(0.0f);
			// If overlapping to the left, move left
			else if (aMin.GetX() <= bMin.GetX() && aMax.GetX() >= bMin.GetX())
				_overlap.SetX(bMin.GetX() - aMax.GetX());
			else if (bMin.GetX() <= aMin.GetX() && bMax.GetX() >= aMin.GetX())
				_overlap.SetX(aMin.GetX() - bMax.GetX());
			// If overlapping to the right, move right
			else if (aMin.GetX() <= bMax.GetX() && aMax.GetX() >= bMax.GetX())
				_overlap.SetX(bMax.GetX() - aMin.GetX());
			else if (bMin.GetX() <= aMax.GetX() && bMax.GetX() >= aMax.GetX())
				_overlap.SetX(aMax.GetX() - bMin.GetX());

			// If completely within the bounds of the other object, don't need to move at all
			if (aMin.GetY() >= bMin.GetY() && aMax.GetY() <= bMax.GetY())
				_overlap.SetY(0.0f);
			else if (bMin.GetY() >= aMin.GetY() && bMax.GetY() <= aMax.GetY())
				_overlap.SetY(0.0f);
			// If overlapping below, move down
			else if (aMin.GetY() <= bMin.GetY() && aMax.GetY() >= bMin.GetY())
				_overlap.SetY(bMin.GetY() - aMax.GetY());
			else if (bMin.GetY() <= aMin.GetY() && bMax.GetY() >= aMin.GetY())
				_overlap.SetY(aMin.GetY() - bMax.GetY());
			// If overlapping above, move up
			else if (aMin.GetY() <= bMax.GetY() && aMax.GetY() >= bMax.GetY())
				_overlap.SetY(bMax.GetY() - aMin.GetY());
			else if (bMin.GetY() <= aMax.GetY() && bMax.GetY() >= aMax.GetY())
				_overlap.SetY(aMax.GetY() - bMin.GetY());

			return true;
		}
	}

	_overlap = Vec2::Zero();
	return false;
}

void CBoxCollider::DebugDraw(float _r, float _g, float _b) const
{
	glDisable(GL_DEPTH_TEST);
	App::DrawLine(m_min.GetX(), m_min.GetY(), m_max.GetX(), m_min.GetY(), _r, _g, _b);
	App::DrawLine(m_max.GetX(), m_min.GetY(), m_max.GetX(), m_max.GetY(), _r, _g, _b);
	App::DrawLine(m_max.GetX(), m_max.GetY(), m_min.GetX(), m_max.GetY(), _r, _g, _b);
	App::DrawLine(m_min.GetX(), m_max.GetY(), m_min.GetX(), m_min.GetY(), _r, _g, _b);
	glEnable(GL_DEPTH_TEST);
}



//--- Setters ---//
void CBoxCollider::SetBaseDimensions(float _width, float _height)
{
	SetBaseDimensions(Vec2(_width, _height));
}

void CBoxCollider::SetBaseDimensions(Vec2 _dimensions)
{
	m_baseDimensions = _dimensions;
}

void CBoxCollider::SetBaseDimensions(CSimpleSprite* _sprite)
{
	SetBaseDimensions(_sprite->GetFrameWidth(), _sprite->GetFrameHeight());
}

void CBoxCollider::SetEntityOffset(float _x, float _y)
{
	SetEntityOffset(Vec2(_x, _y));
}

void CBoxCollider::SetEntityOffset(Vec2 _offset)
{
	m_entityOffset = _offset;
}



//--- Getters ---//
Vec2 CBoxCollider::GetMin() const
{
	return m_min;
}

Vec2 CBoxCollider::GetMax() const
{
	return m_max;
}

void CBoxCollider::GetMinAndMax(Vec2& _min, Vec2& _max) const
{
	_min = m_min;
	_max = m_max;
}



//--- Utility Methods ---//
void CBoxCollider::RecalculateMinAndMax(const Vec2& _entityPos)
{
	Vec2 offsetPos = _entityPos + m_entityOffset;
	Vec2 halfDimensions = m_baseDimensions * 0.5f;
	halfDimensions *= m_transform->GetScale();

	m_max = offsetPos + Vec2(halfDimensions.GetX(), halfDimensions.GetY());
	m_min = offsetPos - Vec2(halfDimensions.GetX(), halfDimensions.GetY());
}

bool CBoxCollider::CheckCollision1D(const Vec2& _line1, const Vec2& _line2)
{
	float line1x = _line1.GetX(), line1y = _line1.GetY();
	float line2x = _line2.GetX(), line2y = _line2.GetY();

	if (line1x > line2x && line1x < line2y) return true;
	if (line1y > line2x && line1y < line2y) return true;

	if (line2x > line1x && line2x < line1y) return true;
	if (line2y > line1x && line2y < line1y) return true;

	return false;
}