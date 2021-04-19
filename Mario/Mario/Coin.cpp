#include "Coin.h"
#include "Character.h"

Coin::Coin(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D start_position) : Character(renderer, imagePath, start_position, map)
{
	//sets up coin 
	m_frame_delay = ANIMATION_DELAY;
	m_position = start_position;
	m_collected = false;
	//splits up sprite sheet 
	m_single_sprite_w = m_texture->GetWidth() / 3;
	m_single_sprite_h = m_texture->GetHeight();
}

Coin::~Coin()
{

}

void Coin::Render()
{
	//choses the correct sprite 
	SDL_Rect portion_of_sprite = { m_single_sprite_w * m_current_frame,0,m_single_sprite_w, m_single_sprite_h };
	SDL_Rect destRect = { (int)(m_position.x), (int)(m_position.y), m_single_sprite_w,m_single_sprite_h };
	m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_NONE);
}

void Coin::Update(float deltaTime, SDL_Event e)
{
	//time before plying next sprite 
	m_frame_delay -= deltaTime;
	if (m_frame_delay <= 0.0f)
	{
		//reset time 
		m_frame_delay = ANIMATION_DELAY;
		m_current_frame++;
		//loops animation 
		if (m_current_frame > 2)
		{
			m_current_frame = 0;
		}
	}
}