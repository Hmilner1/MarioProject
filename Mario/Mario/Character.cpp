#include "Texture2D.h"
#include "Character.h"

Character::Character(SDL_Renderer* renderer, string imagePath, Vector2D start_position)
{
	m_renderer = renderer;
	m_position = start_position;
	m_texture = new Texture2D(m_renderer);
	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
	}
	m_facing_direction = FACING_RIGHT;
	//m_moving_left = false;
	//m_moving_right = false;

}
Character::~Character()
{
	m_renderer = nullptr;
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
	SDL_PollEvent(&e);
	switch (e.key.keysym.sym)
	{
	case SDL_KEYDOWN:
	{
	case SDLK_LEFT:
	{
		m_position.x -= 1;
		m_facing_direction = FACING_LEFT;
		//m_moving_left = true;
		break;
	}
	case SDLK_RIGHT:
	{
		m_position.x += 1;
		m_facing_direction = FACING_RIGHT;
		//m_moving_right = true;
		break;
	}
	}
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
