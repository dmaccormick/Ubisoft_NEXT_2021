#pragma once

// Standard includes
#include <string>

// Internal includes
#include "Scene.h"

class SGame : public Scene
{
public:
	//--- Constructors and Destructor ---//
	SGame();
	~SGame();

	//--- Scene Interface ---//
	virtual void Init() override;
	virtual void Update(float _deltaTime) override;
	virtual void Draw() override;

	//--- Game Logic Methods ---//
	void LoadLevel();

	//--- Setters ---//
	void SetLevelName(std::string _levelName);

private:
	//--- Private Data ---//
	static std::string m_levelFolderPath;
	std::string m_levelName;
	std::vector<Entity*> m_levelPieces;
};