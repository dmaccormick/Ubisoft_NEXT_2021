// Standard header
#include "stdafx.h"

// Internal includes
#include "SEndScreen.h"
#include "CTransform.h"
#include "CSprite.h"
#include "CButton.h"
#include "CLabel.h"
#include "SceneManager.h"
#include "SMenu.h"

//--- Statics ---//
VictoryState SEndScreen::m_victoryState = VictoryState::StillPlaying;



//--- Constructors and Destructor ---//
SEndScreen::SEndScreen()
{
	m_goToMenu = false;
}

SEndScreen::~SEndScreen()
{
}



//--- Scene Interface ---//
void SEndScreen::Init()
{
	// Create the victory / game over message
	{
		auto victoryStateMsg = m_registry.CreateEntity("VictoryStateLabel");

		CTransform* transformComp = m_registry.AddComponent<CTransform>(victoryStateMsg);
		transformComp->SetPosition(512.0f, 400.0f);
		transformComp->SetScale(2.0f);
		
		CSprite* spriteComp = m_registry.AddComponent<CSprite>(victoryStateMsg);
		const char* spriteFile = (m_victoryState == VictoryState::Victory)
								? ".\\GameData\\Sprites\\EndScreen_Victory.bmp"
								: ".\\GameData\\Sprites\\EndScreen_GameOver.bmp";
		spriteComp->LoadSprite(spriteFile);
	}

	// Create the main menu button
	{
		auto mainMenuButton = m_registry.CreateEntity("MainMenuButton");

		CTransform* transformComp = m_registry.AddComponent<CTransform>(mainMenuButton);
		transformComp->SetPosition(512.0f, 100.0f);

		CButton* buttonComp = m_registry.AddComponent<CButton>(mainMenuButton);
		buttonComp->SetDimensions(Vec2(200.0f, 100.0f));
		buttonComp->AddOnClickedCallback(std::bind(&SEndScreen::GoToMenu, this, P_ARG::_1));
		buttonComp->SetColor(Color::Blue());

		CLabel* labelComp = m_registry.AddComponent<CLabel>(mainMenuButton);
		labelComp->SetText("Main Menu");
		labelComp->SetFont(Font::TIMES_ROMAN_24);
		labelComp->SetOffset(Vec2(-60.0f, -10.0f));
	}

	m_registry.InitAll();
}

void SEndScreen::Update(float _deltaTime)
{
	m_registry.UpdateAll(_deltaTime);

	if (m_goToMenu)
		SceneManager::GetInstance()->LoadScene<SMenu>();
}

void SEndScreen::Draw()
{
	auto allSprites = m_registry.GetAllComponentsByType<CSprite>();
	for (auto sprite : allSprites)
		sprite->Draw();

	auto allButtons = m_registry.GetAllComponentsByType<CButton>();
	for (auto button : allButtons)
		button->Draw();

	auto allLabels = m_registry.GetAllComponentsByType<CLabel>();
	for (auto label : allLabels)
		label->Draw();
}



//--- Game Logic Methods ---//
void SEndScreen::GoToMenu(Entity* _button)
{
	m_goToMenu = true;
}



//--- Setters ---//
void SEndScreen::SetVictoryState(VictoryState _victoryState)
{
	m_victoryState = _victoryState;
}
