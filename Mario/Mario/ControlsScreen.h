#pragma once 
#ifndef _CONTROL_SCREEN_H
#define _CONTOL_SCREEN_H
#include "GameScreen.h"
#include "Texture2D.h"
#include <SDL_TTF.h>
#include <iostream>
#include <sstream>

class Texture2D;

class ControlsScreen : GameScreen
{
public:
	ControlsScreen(SDL_Renderer* renderer);
	~ControlsScreen();
	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;

private:
	Texture2D* m_background_texture;
	Texture2D* m_backgroud;
	Texture2D* m_controls;
	bool SetUpLevel();
	TTF_Font* Retro;
	SDL_Rect textRect;
	SDL_Rect backRect;
	SDL_Rect controlRect;

};
#endif