// Standard header
#include "stdafx.h"

// Internal includes
#include "SMenu.h"
#include "CTransform.h"
#include "CButton.h"
#include "CLabel.h"
#include "SGame.h"
#include "SceneManager.h"

//--- Constructors and Destructor ---//
SMenu::SMenu()
{
	m_levelToLoad = "";
	m_skipVictoryState = VictoryState::StillPlaying;
}

SMenu::~SMenu()
{
}



//--- Scene Interface ---//
void SMenu::Init()
{
	// Create the title label
	{
		auto title = m_registry.CreateEntity("Title");

		CTransform* transformComp = m_registry.AddComponent<CTransform>(title);
		transformComp->SetPosition(512.0f, 500.0f);

		CLabel* labelComp = m_registry.AddComponent<CLabel>(title);
		labelComp->SetText("TOWER DEFENSE GAME");
		labelComp->SetFont(Font::TIMES_ROMAN_24);
		labelComp->SetColor(Color::Cyan());
		labelComp->SetOffset(Vec2(-135.0f, 0.0f));
	}

	// Create the level buttons
	for (unsigned int i = 0; i < NUM_LEVELS; i++)
	{
		auto levelButton = m_registry.CreateEntity("Level_" + std::to_string(i + 1));

		CTransform* transformComp = m_registry.AddComponent<CTransform>(levelButton);
		transformComp->SetPosition(512.0f, 400.0f - ((float)i * 55));

		CButton* buttonComp = m_registry.AddComponent<CButton>(levelButton);
		buttonComp->SetColor(Color::Blue());
		buttonComp->AddOnClickedCallback(std::bind(&SMenu::LoadLevel, this, P_ARG::_1));
		buttonComp->SetDimensions(Vec2(200.0f, 50.0f));

		CLabel* labelComp = m_registry.AddComponent<CLabel>(levelButton);
		labelComp->SetText("Load Level " + std::to_string(i + 1));
		labelComp->SetFont(Font::HELVETICA_18);
		labelComp->SetOffset(Vec2(-50.0f, -5.0f));
	}

	// Create the debug buttons to skip to the end screen
	{
		auto debugLabel = m_registry.CreateEntity("DebugLabel");

		CTransform* transformComp = m_registry.AddComponent<CTransform>(debugLabel);
		transformComp->SetPosition(800.0f, 60.0f);

		CLabel* labelComp = m_registry.AddComponent<CLabel>(debugLabel);
		labelComp->SetColor(Color::Magenta());
		labelComp->SetText("Debug Skip To End Screen");
		labelComp->SetFont(Font::HELVETICA_10);
	}
	{
		auto skipWithWinButton = m_registry.CreateEntity("Win");

		CTransform* transformComp = m_registry.AddComponent<CTransform>(skipWithWinButton);
		transformComp->SetPosition(800.0f, 25.0f);

		CButton* buttonComp = m_registry.AddComponent<CButton>(skipWithWinButton);
		buttonComp->SetColor(Color::Green());
		buttonComp->SetDimensions(Vec2(100.0f, 40.0f));
		buttonComp->AddOnClickedCallback(std::bind(&SMenu::SkipToEndScreen, this, P_ARG::_1));

		CLabel* labelComp = m_registry.AddComponent<CLabel>(skipWithWinButton);
		labelComp->SetColor(Color::Green());
		labelComp->SetText("WIN");
		labelComp->SetFont(Font::HELVETICA_10);
		labelComp->SetOffset(Vec2(-10.0f, -3.0));
	}
	{
		auto skipWithLossButton = m_registry.CreateEntity("Loss");

		CTransform* transformComp = m_registry.AddComponent<CTransform>(skipWithLossButton);
		transformComp->SetPosition(920.0f, 25.0f);

		CButton* buttonComp = m_registry.AddComponent<CButton>(skipWithLossButton);
		buttonComp->SetColor(Color::Red());
		buttonComp->SetDimensions(Vec2(100.0f, 40.0f));
		buttonComp->AddOnClickedCallback(std::bind(&SMenu::SkipToEndScreen, this, P_ARG::_1));

		CLabel* labelComp = m_registry.AddComponent<CLabel>(skipWithLossButton);
		labelComp->SetColor(Color::Red());
		labelComp->SetText("LOSS");
		labelComp->SetFont(Font::HELVETICA_10);
		labelComp->SetOffset(Vec2(-10.0f, -3.0));
	}

	m_registry.InitAll();
}

void SMenu::Update(float _deltaTime)
{
	m_registry.UpdateAll(_deltaTime);

	// If there is now a level to load, do so
	// Or if one of the skip buttons was pressed, go to the end screen
	if (m_levelToLoad != "")
	{
		SGame::SetLevelName(m_levelToLoad);
		SceneManager::GetInstance()->LoadScene<SGame>();
	}
	else if (m_skipVictoryState != VictoryState::StillPlaying)
	{
		SEndScreen::SetVictoryState(m_skipVictoryState);
		SceneManager::GetInstance()->LoadScene<SEndScreen>();
	}
}

void SMenu::Draw()
{
	auto allButtons = m_registry.GetAllComponentsByType<CButton>();
	for (auto button : allButtons)
		button->Draw();

	auto allLabels = m_registry.GetAllComponentsByType<CLabel>();
	for (auto label : allLabels)
		label->Draw();
}



//--- Game Logic Methods ---//
void SMenu::LoadLevel(Entity* _button)
{
	// The button's name contains the level to load so use that as a base
	std::string buttonName = _button->GetName();
	std::string levelName = buttonName + ".txt";

	m_levelToLoad = levelName;
}

void SMenu::SkipToEndScreen(Entity* button)
{
	m_skipVictoryState = (button->GetName() == "Win") ? VictoryState::Victory : VictoryState::Loss;
}
