#pragma once
#ifndef _END_SCREEN_H
#define _END_SCREEN_H
#include "GameScreen.h"
#include "GameScreenManager.h"
#include "Commons.h"
#include <SDL_mixer.h>
#include <SDL_TTF.h>
#include <iostream>

class  Texture2D;
class GameOver : GameScreen
{
public:
	GameOver(SDL_Renderer* renderer);
	~GameOver();
	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
	bool SetUpLevel();

private:
	Texture2D* m_background_texture;
	Texture2D* m_background;
	TTF_Font* Retro;
	SDL_Rect GameOverRect;
	SDL_Rect ScoreRect;
	SDL_Rect PlayRect;

};
#endif