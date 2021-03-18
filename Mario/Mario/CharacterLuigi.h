#pragma once
#ifndef _CHARACTER_Luigi_
#define _CHARACTER_Luigi_
#include "Character.h"
class CharacterLuigi : public Character
{
public:
	CharacterLuigi(SDL_Renderer* renderer, string imagePath, Vector2D start_position, LevelMap* map);
	void Update(float deltaTime, SDL_Event e) override;
};
#endif

