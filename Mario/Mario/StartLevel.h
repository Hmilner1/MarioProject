#pragma once
#ifndef _START_SCREEN_H
#define _START_SCREEN_H
#include "GameScreen.h"
#include "GameScreenManager.h"
#include "Commons.h"
#include <vector>
#include <SDL_mixer.h>
class  Texture2D;

class StartLevel : GameScreen
{

public:
	StartLevel(SDL_Renderer* renderer);
	~StartLevel();
	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;

private:
	Texture2D* m_background_texture;
	Texture2D* m_background;
	bool SetUpLevel();

};
#endif