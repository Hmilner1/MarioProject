#pragma once
#ifndef _GOOMBA_
#define _GOOMBA_
#include "Character.h"
#include <SDL_mixer.h>
class Goomba : public Character
{
public:
	Goomba(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D start_position, FACING start_facing, float movement_speed);
	~Goomba();
	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
	void TakeDamage();
	void Turn();
	bool GetInjured();
	void Jump();
	Mix_Chunk* stompSound = Mix_LoadWAV("Music/Stomp.wav");

private:
	void MoveRight(float deltaTime) override;
	void MoveLeft(float deltaTime) override;

	float m_movement_speed;
	bool m_injured;
	float m_frame_delay;
	int m_current_frame;

};
#endif // !_GOOMBA_

