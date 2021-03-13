#include "CharacterLuigi.h"
#include "Character.h"
#include "Texture2D.h"

CharacterLuigi::CharacterLuigi(SDL_Renderer* renderer, string imagePath, Vector2D start_position, LevelMap* map) : Character(renderer, imagePath, start_position, map)
{


}

void CharacterLuigi::Update(float deltaTime, SDL_Event e)
{
	SDL_PollEvent(&e);
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			m_moving_left = true;
			break;
		case SDLK_d:
			m_moving_right = true;
			break;
		case SDLK_w:
			if (m_can_jump)
			{
				Jump();
			}
		}
		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			m_moving_left = false;
			break;
		case SDLK_d:
			m_moving_right = false;
			break;
		}
		break;
	}
	if (m_moving_left == true)
	{
		MoveLeft(deltaTime);

	}

	else if (m_moving_right == true)
	{
		MoveRight(deltaTime);
	}

	Character::Update(deltaTime, e);
}