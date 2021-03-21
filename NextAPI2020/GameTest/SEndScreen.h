#pragma once

// Internal includes
#include "Scene.h"
#include "VictoryState.h"

class SEndScreen : public Scene
{
public:
	//--- Constructors and Destructor ---//
	SEndScreen();
	~SEndScreen();

	//--- Scene Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;
	virtual void Draw() override;

	//--- Game Logic Methods ---//
	void GoToMenu(Entity* _button);

	//--- Setters ---//
	static void SetVictoryState(VictoryState _victoryState);

private:
	//--- Private Data ---//
	static VictoryState m_victoryState;
	bool m_goToMenu;
};