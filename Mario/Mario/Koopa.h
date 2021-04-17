#pragma once
#ifndef _KOOPA_
#define _KOOPA_
#include "Character.h"
#include <SDL_mixer.h>
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
	void Turn();
	Mix_Chunk* stompSound = Mix_LoadWAV("Music/Stomp.wav");
	void MoveRight(float deltaTime) override;
	void MoveLeft(float deltaTime) override;
	bool m_injured;
	float m_movement_speed;
private:
	void FlipRightWayUp();
	float m_injured_time;
};
#endif