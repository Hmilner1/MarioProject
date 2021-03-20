#pragma once
#ifndef _COIN_
#define _COIN_
#include "Character.h"
class Coin : public Character
{
public:
	Coin(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D start_position );
	~Coin();
	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;

private:
	float m_single_sprite_w;
	float m_single_sprite_h;
	float m_collect_time;
	float m_frame_delay;
	int m_current_frame;
	bool m_collected;
	


};
#endif

