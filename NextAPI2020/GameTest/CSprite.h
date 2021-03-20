#pragma once

// Standard includes
#include <string>

// Internal includes
#include "Component.h"
#include "CTransform.h"
#include "App/SimpleSprite.h"

class CSprite : public Component
{
public:
	//--- Constructors and Destructor ---//
	CSprite();
	virtual ~CSprite();

	//--- Component Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;

	//--- Methods ---//
	void LoadSprite(const char* _fileName, int _numCols = 1, int _numRows = 1, float _renderLayer = 0.0f);
	void CreateAnimation(unsigned int _id, float _speed, const std::vector<int>& _frames);
	void DrawSprite();

	//--- Setters ---//
	void SetAnimation(unsigned int _id);
	void SetFrame(unsigned int _frameNum);
	void SetRenderLayer(float _depthLayer);
	void SetColor(float _r, float _g, float _b, float _a = 1.0f);

	//--- Getters ---//
	float GetRenderLayer() const;
	CSimpleSprite* GetSprite() const;

private:
	//--- Private Data ---//
	CTransform* m_transform;
	CSimpleSprite* m_sprite;
};