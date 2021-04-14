#pragma once
#ifndef _CHARACTER_MARIO_
#define _CHARACTER_MARIO_
#include "Character.h"

class CharacterMario : public Character
{
public:
	CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D start_position, LevelMap* map);
	void Update(float deltaTime, SDL_Event e) override;
	void Render() override;
	int Score = 0;
	int lifeCount = 2;
private:
	int frame;
	float frameDelay;
};
#endif

