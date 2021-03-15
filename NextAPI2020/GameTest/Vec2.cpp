// Standard header
#include "stdafx.h"

// Internal includes
#include "Vec2.h"

//--- Constructors and Destructor ---//
Vec2::Vec2()
{
	m_x = 0.0f;
	m_y = 0.0f;
}

Vec2::Vec2(float _x, float _y)
{
	m_x = _x;
	m_y = _y;
}

Vec2::Vec2(float _val)
{
	m_x = _val;
	m_y = _val;
}

Vec2::Vec2(const Vec2& _other)
{
	m_x = _other.GetX();
	m_y = _other.GetY();
}

Vec2::~Vec2()
{

}



//--- Methods ---//
void Vec2::Normalize()
{
	float mag = GetMagnitude();
	m_x /= mag;
	m_y /= mag;
}

float Vec2::Dot(const Vec2& _other) const
{
	return (m_x * _other.GetX()) + (m_y * _other.GetY());
}



//--- Setters ---//
void Vec2::SetX(float _x)
{
	m_x = _x;
}

void Vec2::SetY(float _y)
{
	m_y = _y;
}

void Vec2::Set(float _x, float _y)
{
	m_x = _x;
	m_y = _y;
}

void Vec2::Set(const Vec2& _other)
{
	m_x = _other.GetX();
	m_y = _other.GetY();
}



//--- Getters ---//
float Vec2::GetX() const
{
	return m_x;
}

float Vec2::GetY() const
{
	return m_y;
}

float Vec2::GetMagnitude() const
{
	return sqrt(GetMagnitudeSquared());
}

float Vec2::GetMagnitudeSquared() const
{
	return (m_x * m_x + (m_y * m_y));
}



//--- Operator Overloads ---//
Vec2 Vec2::operator+ (const Vec2& _other) const
{
	return Vec2(m_x + _other.GetX(), m_y + _other.GetY());
}

Vec2& Vec2::operator+= (const Vec2& _other)
{
	*this = *this + _other;
	return *this;
}

Vec2 Vec2::operator- (const Vec2& _other) const
{
	return Vec2(m_x - _other.GetX(), m_y - _other.GetY());
}

Vec2& Vec2::operator-= (const Vec2& _other)
{
	*this = *this - _other;
	return *this;
}

Vec2 Vec2::operator* (const float& _other) const
{
	return Vec2(m_x * _other, m_y * _other);
}

Vec2 operator* (const float& _a, const Vec2& _b)
{
	return _b * _a;
}

Vec2& Vec2::operator*= (const float& _other)
{
	*this = *this * _other;
	return *this;
}

void Vec2::operator= (const Vec2& _other)
{
	m_x = _other.GetX();
	m_y = _other.GetY();
}

bool Vec2::operator== (const Vec2& _other) const
{
	return (m_x == _other.GetX() && m_y == _other.GetY());
}

Vec2 Vec2::operator- () const
{
	return Vec2(-m_x, -m_y);
}

std::ostream& operator<< (std::ostream& _os, const Vec2& _vec)
{
	return _os << "v = [ " << _vec.m_x << " , " << _vec.m_y << " ]";
}



//--- Static Methods ---//
float Vec2::Dot(const Vec2& _a, const Vec2& _b)
{
	return _a.Dot(_b);
}

Vec2& Vec2::Normalize(const Vec2& _vec)
{
	float mag = _vec.GetMagnitude();
	return Vec2(_vec.m_x / mag, _vec.m_y / mag);
}

Vec2& Vec2::Up()
{
	return Vec2(0.0f, 1.0f);
}

Vec2& Vec2::Right()
{
	return Vec2(1.0f, 0.0f);
}

Vec2& Vec2::Down()
{
	return Vec2(0.0f, -1.0f);
}

Vec2& Vec2::Left()
{
	return Vec2(-1.0f, 0.0f);
}

Vec2& Vec2::Zero()
{
	return Vec2();
}