#include "Character.h"

Character::Character(SDL_Renderer* renderer, string imagePath, Vector2D start_position, LevelMap* map)
{
	//sets up the character
	//texture and postition
	m_renderer = renderer;
	m_position = start_position;
	m_texture = new Texture2D(m_renderer);
	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
	}
	//movement 
	m_moving_left = false;
	m_moving_right = false;
	m_facing_direction = FACING_RIGHT;

	m_current_level_map = map;
	m_alive = true;
	canMove = true;
	m_collision_radius = 15.0f;
}

Character::~Character()
{
	delete m_texture;
	m_texture = nullptr;
	m_renderer = nullptr;
	Mix_FreeChunk(jumpSound);
	jumpSound = nullptr;
	delete jumpSound;
}

void Character::Render(int camX, int camY)
{
	// Get the position of the spritesheet
	int left = mSingleSpriteWidth * (frame - 1);

	// Set Rect
	SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };

	// Draw
	if (m_facing_direction == FACING::FACING_RIGHT)
	{
		m_texture->Render(m_position.x - camX, m_position.y - camY, &portionOfSpritesheet, 0.0, nullptr, SDL_FLIP_NONE);

	}
	else
	{
		m_texture->Render(m_position.x - camX, m_position.y - camY, &portionOfSpritesheet, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
	}
}

void Character::Update(float deltaTime, SDL_Event e)
{
	Vector2D old_pos = GetPosition();
	Vector2D new_pos = old_pos;
	canMove = true;

	if (m_position.y >= 438)
	{
		m_alive = false;
	}

	//the players central X and Feet posititon 
	centralX_position = (int)(m_position.x + (m_single_sprite_w * 0.5)) / TILE_WIDTH;
	centralYPositionInGrid = (int)(m_position.y + (m_single_sprite_h * 0.5)) / TILE_HEIGHT;
	foot_position = (int)(m_position.y + m_single_sprite_h) / TILE_HEIGHT;
	head_position = (int)(m_position.y) / TILE_HEIGHT;
	rightSidePositionInGrid = (int)(m_position.x + m_single_sprite_w) / TILE_WIDTH ;
	leftSidePositionInGrid = (int)m_position.x / TILE_WIDTH;
	//if player isnt on a platform add gravity else they can jump
	if (m_current_level_map->GetTileAt(foot_position, centralX_position) == 0)
	{
		AddGravity(deltaTime);
	}
	else
	{
		m_can_jump = true;
	}
	//controls the players jump 
	if (m_jumping)
	{
		m_position.y -= m_jump_force * deltaTime;
		m_jump_force -= JUMP_FORCE_DECREMENT * deltaTime;
		if (m_jump_force <= 0.0f)
			m_jumping = false;
	}
	//allows player to move left and right 
	if (canMove)
	{
		if (m_moving_left)
		{
			MoveLeft(deltaTime);
		}
		else if (m_moving_right)
		{
			MoveRight(deltaTime);
		}
	}


	// Check if the head bumps into a block
	if (m_current_level_map->GetTileAt(head_position, centralX_position) == 1)
	{
		// If the tile is solid, push the player down
		if (m_jumping)
			//AddGravity(deltaTime);
			CancelJump();
	}

	// Sideway collision
	if (m_facing_direction == FACING::FACING_RIGHT)
	{
		// If the right side collides with a solid tile, stop movement
		if (m_current_level_map->GetTileAt(centralYPositionInGrid, rightSidePositionInGrid) == 1)
		{
			canMove = false;
			m_position = Vector2D(old_pos.x - 1, old_pos.y);
		}
		
	}
	else if (m_facing_direction == FACING::FACING_LEFT)
	{
		// If the left side collides with a solid tile, stop movement
		if (m_current_level_map->GetTileAt(centralYPositionInGrid, leftSidePositionInGrid) == 1)
		{
			canMove = false;
			m_position = Vector2D(old_pos.x + 1, old_pos.y);
		}
		
	}
	
}

Vector2D Character::GetPosition()
{
	return m_position;
}

void Character::SetPosition(Vector2D newPosition)
{
	m_position = newPosition;
}

void Character::MoveLeft(float deltaTime)
{
	//changes facing direction x axis movement 
	m_facing_direction = FACING_LEFT;
	m_position.x -= MOVEMENTSPEED* deltaTime;
}

void Character::MoveRight(float deltaTime)
{
	//changes facing direction x axis movement 
	m_facing_direction = FACING_RIGHT;
	m_position.x += MOVEMENTSPEED* deltaTime;
}

void Character::AddGravity(float deltaTime)
{
	//cotrols the amount of gravity applied to mario
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
	//controls the jump character is not jumping
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

void Character::Dead(float deltaTime)
{
	//controls what to do after character dies 
	m_alive = false;
	m_position.x = -1000;
	m_position.y = -1000;
	m_can_jump = false;
}

void Character::hop()
{
	//controls the hop after hitting an enime
	m_jump_force = HOP;
	m_jumping = true;
	m_can_jump = false;
}