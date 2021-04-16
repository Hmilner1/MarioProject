#include "CharacterMario.h"
#include "Character.h"

CharacterMario::CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D start_position, LevelMap* map) : Character(renderer, imagePath, start_position, map)
{
	frame = 6;
	m_single_sprite_w = m_texture->GetWidth() / frame;
	m_single_sprite_h = m_texture->GetHeight();
}

void CharacterMario::Update(float deltaTime, SDL_Event e)
{
	if (m_moving_left == true || m_moving_right == true)
	{
		frameDelay -= deltaTime;
		if (frameDelay <= 0.0f)
		{
			frameDelay = CHARANIMATION_DELAY;

			frame++;

			if (frame > 3)
			{
				frame = 0;
			}
		}
	}

	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			m_moving_left = true;
			m_moving_right = false;
			break;
		case SDLK_RIGHT:
			m_moving_right = true;
			m_moving_left = false;
			break;
		case SDLK_UP:
			if (m_can_jump)
			{
				Jump();
			}
		}
		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			m_moving_left = false;
			frame = 0;
			break;
		case SDLK_RIGHT:
			m_moving_right = false;
			frame = 0;
			break;
		}
	}
	if (m_moving_left)
	{
		MoveLeft(deltaTime);
	}
	else if (m_moving_right)
	{
		MoveRight(deltaTime);
	}

	if (m_jumping == true)
	{
		frame = 4;
	}
	else if(frame > 3 && m_jumping == false)
	{
		frame = 0;
	}
	Character::Update(deltaTime, e);
}

void CharacterMario::Render()
{
	int left = m_single_sprite_w * frame;
	SDL_Rect portion_of_sprite = { left, 0, m_single_sprite_w, m_single_sprite_h };
	SDL_Rect destRect = { (int)(m_position.x), (int)(m_position.y), m_single_sprite_w, m_single_sprite_h };
	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_HORIZONTAL);
	}
	else if (m_facing_direction == FACING_LEFT)
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_NONE);
	}
}















