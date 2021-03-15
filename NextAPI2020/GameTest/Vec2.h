#pragma once

// Standard Includes
#include <iostream>

class Vec2
{
public:
	//--- Constructors and Destructor ---//
	Vec2();
	Vec2(float _x, float _y);
	Vec2(float _val);
	Vec2(const Vec2& _other);
	~Vec2();

	//--- Methods ---//
	void Normalize();
	float Dot(const Vec2& _other) const;

	//--- Setters ---//
	void SetX(float _x);
	void SetY(float _y);
	void Set(float _x, float _y);
	void Set(const Vec2& _vec);

	//--- Getters ---//
	float GetX() const;
	float GetY() const;
	float GetMagnitude() const;
	float GetMagnitudeSquared() const;

	//--- Operator Overloads ---//
	Vec2&					operator+	(const Vec2& _other) const;
	Vec2&					operator+=	(const Vec2& _other);
	Vec2&					operator-	(const Vec2& _other) const;
	Vec2&					operator-=	(const Vec2& _other);
	Vec2&					operator*	(const float& _other) const;
	friend Vec2&			operator*	(const float& _a, const Vec2& _b);
	Vec2&					operator*=	(const float& _other);
	void					operator=	(const Vec2& _other);
	bool					operator==	(const Vec2& _other) const;
	Vec2&					operator-	() const;
	friend std::ostream&	operator<<	(std::ostream& _os, const Vec2& _vec);

	//--- Static Methods ---//
	static float Dot(const Vec2& _a, const Vec2& _b);
	static Vec2& Normalize(const Vec2& _vec);
	static Vec2& Up();
	static Vec2& Right();
	static Vec2& Down();
	static Vec2& Left();
	static Vec2& Zero();

private:
	//--- Data Members ---//
	float m_x;
	float m_y;
};