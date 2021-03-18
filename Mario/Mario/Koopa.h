#pragma once
#ifndef _KOOPA_
#define _KOOPA_
#include "Character.h"
class Koopa: public Character
{
public: 
	Koopa(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D start_position, FACING start_facing, float movement_speed);
	~Koopa();
	void TakeDamage();
	void Jump();
	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
	bool GetInjured();
private:
	void FlipRightWayUp();
	void MoveRight(float deltaTime) override;
	void MoveLeft(float deltaTime) override;

	float m_single_sprite_w;
	float m_single_sprite_h;
	float m_movement_speed;
	bool m_injured;
	bool m_injured_time;
};
#endif

