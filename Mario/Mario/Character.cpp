//#include "Texture2D.h"
#include "Character.h"

Character::Character(SDL_Renderer* renderer, string imagePath, Vector2D start_position, LevelMap* map)
{
	m_renderer = renderer;
	m_position = start_position;
	m_texture = new Texture2D(m_renderer);
	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
	}
	m_moving_left = false;
	m_moving_right = false;
	m_collision_radius = 15.0f;
	m_facing_direction = FACING_RIGHT;
	m_current_level_map = map;
	m_alive = true;
}
Character::~Character()
{
	m_renderer = nullptr;
	Mix_FreeChunk(jumpSound);
	jumpSound = nullptr;
	delete jumpSound;
}

void Character::Render()
{
	m_texture->Render(Vector2D(m_position), SDL_FLIP_NONE);
	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(m_position, SDL_FLIP_NONE);
	}
	else
	{
			m_texture->Render(m_position, SDL_FLIP_HORIZONTAL);
	}
}
void Character::Update(float deltaTime, SDL_Event e)
{
	//collision position variables
	centralX_position = (int)(m_position.x + (m_single_sprite_w * 0.5)) / TILE_WIDTH;
	foot_position = (int)(m_position.y + m_single_sprite_h) / TILE_HEIGHT;
	//deal with gravity
	if (m_current_level_map->GetTileAt(foot_position, centralX_position) == 0)
	{
		AddGravity(deltaTime);
	}
	else
	{
		//collided with ground so we can jump again
		m_can_jump = true;
	}
	//deal with jumping first
	if (m_jumping)
	{
		//adjust position
		m_position.y -= m_jump_force * deltaTime;
		//reduce jump force
		m_jump_force -= JUMP_FORCE_DECREMENT * deltaTime;
		//is jump force 0?
		if (m_jump_force <= 0.0f)
			m_jumping = false;
	}

	if (m_moving_left)
	{
		MoveLeft(deltaTime);
	}
	else if (m_moving_right)
	{
		MoveRight(deltaTime);
	}
}
void Character::SetPosition(Vector2D new_position)
{
	m_position = new_position;
}

Vector2D Character::GetPosition()
{
	return m_position;
}

void Character::MoveLeft(float deltaTime)
{
	m_facing_direction = FACING_LEFT;
	m_position.x -= MOVEMENTSPEED* deltaTime;
}

void Character::MoveRight(float deltaTime)
{
	m_facing_direction = FACING_RIGHT;
	m_position.x += MOVEMENTSPEED* deltaTime;
}

void Character::AddGravity(float deltaTime)
{
	if (m_position.y + 64 <= SCREEN_HEIGHT)
	{
		m_position.y += GRAVITY * deltaTime;
	}
	else
	{
		m_can_jump = true;
	}
}

void Character::Jump()
{
	if (!m_jumping)
	{
		m_jump_force = INITIAL_JUMP_FORCE;
		Mix_PlayChannel(-1, jumpSound, 0);
		m_jumping = true;
		m_can_jump = false;
	}
}

float Character::GetCollisionRadius()
{
	return m_collision_radius;
}

void Character::SetAlive(bool isAlive)
{
	m_alive = isAlive;
}

void Character::Dead()
{
	m_alive = false;
	m_position.x = -1000;
	m_position.y = -1000;
}

void Character::hop()
{
	m_jump_force = HOP;
	m_jumping = true;
	m_can_jump = false;
}
