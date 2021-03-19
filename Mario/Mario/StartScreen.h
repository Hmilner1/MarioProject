#pragma once
#ifndef _STARTSCREEN_H
#define _STARTSCREEM_H
#include "GameScreen.h"
#include "GameScreenManager.h"
#include "Commons.h"
class  Texture2D;
class StartScreen : GameScreen
{

public:
	StartScreen(SDL_Renderer* renderer);
	~StartScreen();

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
	bool SetUpLevel();


private:
	Texture2D* m_background_texture;
	Texture2D* m_background;

};
#endif
