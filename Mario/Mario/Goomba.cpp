#include "Goomba.h"

Goomba::Goomba(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D start_position, FACING start_facing, float movement_speed) : Character(renderer, imagePath, start_position, map)
{
	m_frame_delay = 0.50;
	m_facing_direction = start_facing;
	m_movement_speed = movement_speed;
	m_position = start_position;
	m_injured = false;
	m_single_sprite_w = m_texture->GetWidth() / 2;
	m_single_sprite_h = m_texture->GetHeight();
}

Goomba::~Goomba()
{

}

void Goomba::Render()
{
	//int left = 0.0f;
	//left = m_single_sprite_w;
	SDL_Rect portion_of_sprite = { m_single_sprite_w * m_current_frame,0,m_single_sprite_w, m_single_sprite_h };
	SDL_Rect destRect = { (int)(m_position.x), (int)(m_position.y), m_single_sprite_w,m_single_sprite_h };
	m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_NONE);
}

void Goomba::Update(float deltaTime, SDL_Event e)
{
	m_frame_delay -= deltaTime;
	if (m_frame_delay <= 0.0f)
	{
		//reset frame delay count
		m_frame_delay = 0.50;
		//move the frame over
		m_current_frame++;
		//loop frame around if it goes beyond the number of frames
		if (m_current_frame > 1)
			m_current_frame = 0;
	}
	Character::Update(deltaTime, e);
	//use the code within the base class
	if (!m_injured)
	{
		//enemy is not injured so move
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
	m_injured = true;
	Jump();
}

void Goomba::Turn()
{
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
