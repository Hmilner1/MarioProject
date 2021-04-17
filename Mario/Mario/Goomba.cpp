#include "Goomba.h"

Goomba::Goomba(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D start_position, FACING start_facing, float movement_speed) : Character(renderer, imagePath, start_position, map)
{
	m_frame_delay = 0.50;
	m_facing_direction = start_facing;
	m_movement_speed = movement_speed;
	m_position = start_position;
	m_injured = false;
	//splits up sprite sheet
	m_single_sprite_w = m_texture->GetWidth() / 2;
	m_single_sprite_h = m_texture->GetHeight();
}

Goomba::~Goomba()
{
	Mix_FreeChunk(stompSound);
	stompSound = nullptr;
	delete stompSound;
}

void Goomba::Render()
{
	//pushes the correct sprite to the renderer
	SDL_Rect portion_of_sprite = { m_single_sprite_w * m_current_frame,0,m_single_sprite_w, m_single_sprite_h };
	SDL_Rect destRect = { (int)(m_position.x), (int)(m_position.y), m_single_sprite_w,m_single_sprite_h };
	m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_NONE);
}

void Goomba::Update(float deltaTime, SDL_Event e)
{
	//sets an animation delay between the sprites
	m_frame_delay -= deltaTime;
	if (m_frame_delay <= 0.0f)
	{
		//resets delay
		m_frame_delay = 0.50;
		//pushes to next frame
		m_current_frame++;
		//resets frame once its gone through the cycle 
		if (m_current_frame > 1)
			m_current_frame = 0;
	}
	Character::Update(deltaTime, e);
	//if goomba isnt damaged then allow it to walk 
	if (!m_injured)
	{
		if (m_facing_direction == FACING_LEFT)
		{
			m_moving_left = true;
			m_moving_right = false;
		}
		else if (m_facing_direction == FACING_RIGHT)
		{
			m_moving_right = true;
			m_moving_left = false;
		}
	}
}

void Goomba::TakeDamage()
{
	//controls what happens once damaged 
	m_injured = true;
	Mix_PlayChannel(-1, stompSound, 0);
	Jump();
}

void Goomba::Turn()
{
	//makes the goomba turn 
	if (m_facing_direction == FACING_LEFT)
	{
		m_facing_direction = FACING_RIGHT;
		m_moving_right = true;
		m_moving_left = false;
	}
	else if (m_facing_direction == FACING_RIGHT)
	{
		m_facing_direction = FACING_LEFT;
		m_moving_right = false;
		m_moving_left = true;
	}
}

bool Goomba::GetInjured()
{
	return m_injured;
}

void Goomba::MoveLeft(float deltaTime)
{
	m_facing_direction = FACING_LEFT;
	m_position.x -= m_movement_speed * deltaTime;
}

void Goomba::MoveRight(float deltaTime)
{
	m_facing_direction = FACING_RIGHT;
	m_position.x += m_movement_speed * deltaTime;
}

void Goomba::Jump()
{
	m_jumping = true;
	m_can_jump = false;
	m_jump_force = INITIAL_JUMP_FORCE_SMALL;
}