#pragma once
#ifndef _SCORE_SCREEN_
#define _SCORE_SCREEN_
#include "GameScreen.h"
#include "Texture2D.h"
#include <SDL_TTF.h>
#include <iostream>
#include <sstream>
#include "Collisions.h"
#include <SDL_TTF.h>
#include <vector>


class Texute2D;

class ScoreScreen : GameScreen 
{
public:
	ScoreScreen(SDL_Renderer* renderer);
	~ScoreScreen();
	void Render() override;
	void Update(float deltaTime, SDL_Event e);
private:
	Texture2D* m_background_texture;
	bool SetUpLevel();
	TTF_Font* Retro;
	SDL_Rect textRect;
	SDL_Rect cameraRect;
	int hightScore;
};
#endif 

