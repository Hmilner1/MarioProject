#include "CharacterLuigi.h"


CharacterLuigi::CharacterLuigi(SDL_Renderer* renderer, string imagePath, Vector2D start_position, LevelMap* map) : Character(renderer, imagePath, start_position, map)
{
	//splits the sprite sheet into individual sprites
	m_single_sprite_w = m_texture->GetWidth() / 6;
	m_single_sprite_h = m_texture->GetHeight();
}

void CharacterLuigi::Update(float deltaTime, SDL_Event e)
{
	//if the player is moving then move the sprite along the sheet
	if (m_moving_left == true || m_moving_right == true)
	{
		//adds time between frames
		frameTime -= deltaTime;
		if (frameTime <= 0.0f)
		{
			//reset time 
			frameTime = CHARANIMATION_DELAY;
			frame++;
			//if at the end of the animation reset to the start 
			if (frame > 3)
			{
				frame = 0;
			}
		}
	}

	//player movement controls for luigi
	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			m_moving_left = true;
			m_moving_right = false;
			break;
		case SDLK_d:
			m_moving_right = true;
			m_moving_left = false;
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
			frame = 0;
			break;
		case SDLK_d:
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
	//move to a diffrent fram if player jumps and resets once landed
	if (m_jumping == true)
	{
		frame = 4;
	}
	else if (m_jumping == false && frame >= 3)
	{
		frame = 0;
	}
	//call the rest of the update
	Character::Update(deltaTime, e);
}

void CharacterLuigi::Render()
{
	//render a single frame and works out which one is to the left of it to play next 
	int left = m_single_sprite_w * frame;
	SDL_Rect portion_of_sprite = { left, 0, m_single_sprite_w, m_single_sprite_h };
	SDL_Rect destRect = { (int)(m_position.x), (int)(m_position.y), m_single_sprite_w, m_single_sprite_h };
	//flips the sprite sheet if facing a diffrent direction 
	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_HORIZONTAL);
	}
	else if (m_facing_direction == FACING_LEFT)
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_NONE);
	}
}