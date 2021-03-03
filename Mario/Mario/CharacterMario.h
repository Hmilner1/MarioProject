#pragma once
#include "Character.h"
#include <SDL.h>
#include <iostream>
#include "Commons.h"
#ifndef _CHARACTER_MARIO_
#define _CHARACTER_MARIO_
class CharacterMario : public Character
{
	CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D start_position);
};
#endif

