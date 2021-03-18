#pragma once

// Internal includes
#include "Scene.h"

class STest : public Scene
{
public:
	//--- Constructors and Destructor ---//
	STest();
	virtual ~STest();

	//--- Scene Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;
	virtual void Draw() override;

private:
	//--- Private Data ---//
	Entity* m_player;
};