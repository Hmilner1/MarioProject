#pragma once
#ifndef _CHARACTER_
#define _CHARACTER_
#include <SDL.h>
#include <iostream>
#include "Commons.h"
#include "Texture2D.h"
#include "LevelMap.h"
#include <SDL_mixer.h>
using namespace std;

class Texture2D;
class Character
{
public:
	Character(SDL_Renderer* renderer, string imagePath, Vector2D start_position, LevelMap* map);
	~Character();
	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	//player events
	virtual void AddGravity(float deltaTime);
	bool IsJumping() { return m_jumping; };
	void CancelJump() { m_jumping = false; }
	virtual void hop();
	virtual void Jump();
	//full sprite sheet 
	Texture2D* m_texture;
	SDL_Renderer* m_renderer;
	//collision
	float GetCollisionRadius();
	Rect2D GetCollisionBox() 
	{
		return Rect2D(m_position.x, m_position.y,m_texture->GetWidth(), m_texture->GetHeight());
	}
	//characters position 
	Vector2D GetPosition();
	int foot_position;
	int centralX_position;
	//controls if character is alive
	bool m_alive;
	void Dead(float deltaTime);
	void SetAlive(bool isAlive);
	bool GetAlive() { return m_alive; }
	FACING m_facing_direction;
	Vector2D m_position;
	bool m_moving_left;
	bool m_moving_right;

private:
	//music
	Mix_Chunk* jumpSound = Mix_LoadWAV("Music/Jump.wav");
protected:
	//player movement
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	bool m_jumping;
	bool m_can_jump;
	float m_jump_force;
	float movement_speed;
	//single sprite size 
	float m_single_sprite_w;
	float m_single_sprite_h;
	//collision
	float m_collision_radius;
	LevelMap* m_current_level_map;
};
#endif