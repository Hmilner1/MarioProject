#pragma once
#ifndef _COLLISIONS_
#define _COLLISIONS_
#include <SDL.h>
#include "commons.h"
class Character;

class Collisions
{
public:
	~Collisions();
	static Collisions* Instance();
	bool Circle(Character* character1, Character* character2);
	bool Box(Rect2D rect1, Rect2D rect2);

private:
	Collisions();
	static Collisions* m_instance;
};
#endif

