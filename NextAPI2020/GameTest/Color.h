#pragma once

struct Color
{
	//--- Constructors ---//
	Color()
	{
		r = 1.0f;
		g = 1.0f;
		b = 1.0f;
		a = 1.0f;
	}

	Color(float _r, float _g, float _b, float _a = 1.0f)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	Color(Color& _other, float _a)
	{
		r = _other.r;
		g = _other.g;
		b = _other.b;

		a = _a;
	}

	//--- Methods ---//
	static Color White	(float _a = 1.0f)	{ return Color(1.0f, 1.0f, 1.0f, _a); }
	static Color Black	(float _a = 1.0f)	{ return Color(0.0f, 0.0f, 0.0f, _a); }
	static Color Red	(float _a = 1.0f)	{ return Color(1.0f, 0.0f, 0.0f, _a); }
	static Color Green	(float _a = 1.0f)	{ return Color(0.0f, 1.0f, 0.0f, _a); }
	static Color Blue	(float _a = 1.0f)	{ return Color(0.0f, 0.0f, 1.0f, _a); }
	static Color Yellow	(float _a = 1.0f)	{ return Color(1.0f, 1.0f, 0.0f, _a); }
	static Color Cyan	(float _a = 1.0f)	{ return Color(0.0f, 1.0f, 1.0f, _a); }
	static Color Magenta(float _a = 1.0f)	{ return Color(1.0f, 0.0f, 1.0f, _a); }
	static Color Brown	(float _a = 1.0f)	{ return Color(0.7f, 0.5f, 0.0f, _a); }

	//--- Public Data ---//
	float r, g, b, a;
};