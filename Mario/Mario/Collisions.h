#pragma once
#ifndef _COLLISIONS_
#define _COLLISIONS_
#include <SDL.h>
#include "commons.h"
#include "Character.h"
#include "CharacterMario.h"
class Character;
class CharacterMario;

class Collisions
{
public:
	Collisions();
	~Collisions();
	static Collisions* Instance();
	bool Circle(Character* character1, Character* character2);
	bool Box(Rect2D rect1, Rect2D rect2);
	CharacterMario* character;

private:
	static Collisions* m_instance;
};
#endif

