#pragma once
#ifndef _CHARACTER_
#define _CHARACTER_
#include <SDL.h>
#include <iostream>
#include "Commons.h"
#include "Texture2D.h"
#include "LevelMap.h"
using namespace std;

class Texture2D;

class Character
{

public:
	Character(SDL_Renderer* renderer, string imagePath, Vector2D start_position, LevelMap* map);
	~Character();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	virtual void AddGravity(float deltaTime);
	void SetPosition(Vector2D new_position);
	bool IsJumping() { return m_jumping; }
	void SetAlive(bool isAlive);
	void Dead();
	bool GetAlive() { return m_alive; }
	void CancelJump() { m_jumping = false; }
	Vector2D GetPosition();
	Texture2D* m_texture;
	SDL_Renderer* m_renderer;
	float GetCollisionRadius();
	virtual void hop();
	Rect2D GetCollisionBox() 
	{
		return Rect2D(m_position.x, m_position.y,m_texture->GetWidth(), m_texture->GetHeight());
	}
	bool m_moving_left;
	bool m_moving_right;
	int foot_position;
private:
	LevelMap* m_current_level_map;

protected:
	virtual void Jump();
	FACING m_facing_direction;
	Vector2D m_position;
	bool m_jumping;
	bool m_can_jump;
	bool m_alive;
	float m_jump_force;
	float m_collision_radius;
	float movement_speed;
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
};
#endif // !_CHARACTER_

