#include "GameScreenManager.h"
#include "GameScreen.h"
#include "GameScreenLevel1.h"
#include "StartLevel.h"

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen)
{
	m_renderer = renderer;
	m_current_screen = nullptr;
	ChangeScreen(startScreen);
}
GameScreenManager::~GameScreenManager()
{
	m_renderer = nullptr;
	delete m_current_screen;
	m_current_screen = nullptr;
}

void GameScreenManager::Render()
{
	m_current_screen-> Render();
}
void GameScreenManager::Update(float deltaTime, SDL_Event e)
{
	m_current_screen->Update(deltaTime, e);

}
void GameScreenManager::ChangeScreen(SCREENS new_Screen)
{
	if (m_current_screen != nullptr)
	{
		delete m_current_screen;
	}

	switch (new_Screen)
	{
	case SCREEN_INTRO:
		StartLevel* tempScreen;
		tempScreen = new StartLevel(m_renderer);
		m_current_screen = (GameScreen*)tempScreen;
		tempScreen = nullptr;
		break;

	case SCREEN_LEVEL1:
		GameScreenLevel1* Level1;
		Level1 = new GameScreenLevel1(m_renderer);
		m_current_screen = (GameScreen*)Level1;
		Level1 = nullptr;
		break;
	default:
		break;
	}
}
