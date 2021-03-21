#pragma once

// Standard includes
#include <string>
#include <map>

// Internal includes
#include "Component.h"
#include "CTransform.h"
#include "Vec2.h"
#include "Color.h"

enum class Font
{
	BASE_9_BY_15,
	BASE_8_BY_13,
	TIMES_ROMAN_10,
	TIMES_ROMAN_24,
	HELVETICA_10,
	HELVETICA_12,
	HELVETICA_18
};

class CLabel : public Component
{
public:
	//--- Constructors and Destructor ---//
	CLabel();
	virtual ~CLabel();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Methods ---//
	void Draw() const;

	//--- Setters ---//
	void SetText(std::string _text);
	void SetOffset(Vec2 _offset);
	void SetActive(bool _active);
	void SetColor(Color _color);
	void SetFont(Font _font);

	//--- Getters ---//
	std::string GetText() const;
	Vec2 GetOffset() const;
	bool GetActive() const;

private:
	//--- Private Data ---//
	static std::map<Font, void*> m_fontMapping;
	CTransform* m_transform;
	Vec2 m_offset;
	std::string m_text;
	bool m_active;
	Color m_color;
	Font m_font;
};