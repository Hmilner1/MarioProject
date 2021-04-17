#pragma once
#ifndef _GAMESCREENMANAGER_H
#define  _GAMESCREENMANAGER_H
#include "GameScreen.h"
#include "StartLevel.h"
#include "GameScreenLevel1.h"
#include "GameScreenLevel2.h"
#include "GameOver.h"

class GameScreen;
class GameScreenManager
{
public:
	GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen);
	~GameScreenManager();
	void Render();
	void Update(float deltaTime, SDL_Event e);
	void ChangeScreen(SCREENS new_Screen);
	GameScreen* m_current_screen;

private:
	SDL_Renderer* m_renderer;
};
#endif