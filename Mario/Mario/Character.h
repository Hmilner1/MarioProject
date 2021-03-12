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
	Vector2D GetPosition();
	Texture2D* m_texture;
	SDL_Renderer* m_renderer;
	float GetCollisionRadius();
	Rect2D GetCollisionBox() 
	{
		return Rect2D(m_position.x, m_position.y,
			m_texture->GetWidth(), m_texture->GetHeight());
	}

private:
	Vector2D m_position;
	FACING m_facing_direction;
	LevelMap* m_current_level_map;

protected:
	bool m_moving_left;
	bool m_moving_right;
	bool m_jumping;
	bool m_can_jump;
	float m_jump_force;
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	virtual void Jump();
	float m_collision_radius;
};
#endif // !_CHARACTER_

