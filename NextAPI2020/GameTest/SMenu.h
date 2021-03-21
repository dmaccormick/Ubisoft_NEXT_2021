#pragma once

// Standard includes
#include <string>

// Internal includes
#include "Scene.h"
#include "VictoryState.h"

#define NUM_LEVELS 4

class SMenu : public Scene
{
public:
	//--- Constructors and Destructor ---//
	SMenu();
	~SMenu();

	//--- Scene Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;
	virtual void Draw() override;

	//--- Game Logic Methods ---//
	void LoadLevel(Entity* _button);
	void SkipToEndScreen(Entity* _button); // Showcase feature just to make it easy to jump to the end screen during demonstration

private:
	//--- Private Data ---//
	std::string m_levelToLoad;
	VictoryState m_skipVictoryState; 
};