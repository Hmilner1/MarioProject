#include "Koopa.h"

Koopa::Koopa(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D start_position, FACING start_facing, float movement_speed) : Character(renderer, imagePath, start_position, map)
{
	m_facing_direction = start_facing;
	m_movement_speed = movement_speed;
	m_position = start_position;
	m_injured = false;
	//spilts up sprite sheet
	m_single_sprite_w = m_texture->GetWidth() / 2;
	m_single_sprite_h = m_texture->GetHeight();
}

Koopa::~Koopa()
{
	Mix_FreeChunk(stompSound);
	stompSound = nullptr;
	delete stompSound;
}

void Koopa::Render(int camX, int camY)
{
	//renders the correct sprite from the sheet 
	int left = 0.0f;
	if (m_injured)
	{
		left = m_single_sprite_w;
	}
	SDL_Rect portion_of_sprite = { left, 0, m_single_sprite_w, m_single_sprite_h };
	SDL_Rect destRect = { (int)(m_position.x), (int)(m_position.y), m_single_sprite_w, m_single_sprite_h };
	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(m_position.x - camX, m_position.y - camY, &portion_of_sprite, 0.0, nullptr, SDL_FLIP_NONE);
	}
	else
		{
		m_texture->Render(m_position.x - camX, m_position.y - camY, &portion_of_sprite, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
	}
}

void Koopa::Update(float deltaTime, SDL_Event e)
{
	Character::Update(deltaTime, e);
	// Sideway collision
	if (m_facing_direction == FACING::FACING_RIGHT)
	{
		// If the right side collides with a solid tile, stop movement
		if (m_current_level_map->GetTileAt(centralYPositionInGrid, rightSidePositionInGrid) == 1 || m_current_level_map->GetTileAt(centralYPositionInGrid, rightSidePositionInGrid) == 2)
		{
			Turn();
		}

	}
	else if (m_facing_direction == FACING::FACING_LEFT)
	{
		// If the left side collides with a solid tile, stop movement
		if (m_current_level_map->GetTileAt(centralYPositionInGrid, leftSidePositionInGrid) == 1 || m_current_level_map->GetTileAt(centralYPositionInGrid, leftSidePositionInGrid) == 2)
		{
			Turn();
		}

	}
	//if the koopa isnt damaged then move depending on the facing direction 
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
	else if(m_injured)
	{
		//countdown before the koopa can flip the correct way again 
		m_injured_time -= deltaTime;
		if (m_injured_time <= 0.0f)
		{
			FlipRightWayUp();
		}
	}
}

void Koopa::TakeDamage()
{
	//what to do once the koopa has taken damage 
	m_injured = true;
	Jump();
	Mix_PlayChannel(-1, stompSound, 0);
	m_moving_right = false;
	m_moving_left = false;
	m_injured_time = INJURED_TIME;
}

void Koopa::Jump()
{
	//hop the shell in the air once it has taken damge 
	m_jumping = true;
	m_can_jump = false;
	m_jump_force = INITIAL_JUMP_FORCE_SMALL;
}

void Koopa::FlipRightWayUp()
{
	//allows the koopa to flip the correct way and change the direction that it is facing 
	cout << "called" << endl;
	if (m_facing_direction == FACING_LEFT)
	{
		m_facing_direction = FACING_RIGHT;
	}
	else if (m_facing_direction == FACING_RIGHT)
	{
		m_facing_direction = FACING_LEFT;
	}
	Jump();
	m_injured = false;
}

bool Koopa::GetInjured()
{
	return m_injured;
}

void Koopa::MoveLeft(float deltaTime)
{
	m_facing_direction = FACING_LEFT;
	m_position.x -= m_movement_speed * deltaTime;
	if (m_injured)
	{
		m_facing_direction = FACING_LEFT;
		m_position.x -= 100 * deltaTime;
	}
}

void Koopa::MoveRight(float deltaTime)
{
	m_facing_direction = FACING_RIGHT;
	m_position.x += m_movement_speed * deltaTime;
	if (m_injured)
	{
		m_facing_direction = FACING_RIGHT;
		m_position.x += 100 * deltaTime;
	}
}

void Koopa::Turn()
{
	//turns the koopa in the otehr direction once called
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