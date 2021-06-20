#pragma once
#ifndef _CHARACTER_MARIO_
#define _CHARACTER_MARIO_
#include "Character.h"
#include "Camera.h"

class CharacterMario : public Character
{
public:
	CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D start_position, LevelMap* map);
	void Update(float deltaTime, SDL_Event e) override;
	void Render(int camX, int camY) override;
	//holds gameplay elements
	int Score = 0;
	int lifeCount = 2;
private:
	int frame;
	float frameTime;
};
#endif