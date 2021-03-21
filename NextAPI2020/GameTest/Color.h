#pragma once

struct Color
{
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

	static Color Red()		{ return Color(1.0f, 0.0f, 0.0f, 1.0f); }
	static Color Blue()		{ return Color(0.0f, 1.0f, 0.0f, 1.0f); }
	static Color Green()	{ return Color(0.0f, 0.0f, 1.0f, 1.0f); }

	static Color Yellow()	{ return Color(1.0f, 1.0f, 0.0f, 1.0f); }
	static Color Cyan()		{ return Color(0.0f, 1.0f, 1.0f, 1.0f); }
	static Color Magenta()	{ return Color(1.0f, 0.0f, 1.0f, 1.0f); }

	float r, g, b, a;
};