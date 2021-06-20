#pragma once
#ifndef _POWBLOCK_
#define _POWBLOCK_
//#include "GameScreen.h"
#include "GameScreenLevel1.h"
class PowBlock
{
public:

	PowBlock(SDL_Renderer* renderer, LevelMap* map);
	~PowBlock();
	void Render(int camX, int camY);
	Rect2D GetCollisionBox()
	{
		return Rect2D(m_position.x, m_position.y, m_texture->GetWidth() /2 , m_texture->GetHeight());
	}
	void TakeHit();
	bool IsAvailable() { return m_num_hits_left > 0; }
	Vector2D m_position;

private:
	SDL_Renderer* m_renderer;
	Texture2D* m_texture;
	LevelMap* m_level_map;

	float m_single_sprite_w;
	float m_single_sprite_h;
	int m_num_hits_left;

};
#endif