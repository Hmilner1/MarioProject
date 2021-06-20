#pragma once
struct Vector2D
{
	float x;
	float y;
	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	};
	
	Vector2D(float initial_x, float initial_y)
	{
		x = initial_x;
		y = initial_y;
	}
};
enum SCREENS
{
	SCREEN_INTRO,
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES,
	SCREEN_CONTROLS
};

enum class CollisionType
{
	TILE_WALKABLE,
	TILE_NONWALKABLE,
	TILE_AIR,
	TILE_SOLID,
	TILE_PLATFORM
};


enum FACING
{
	FACING_LEFT,
	FACING_RIGHT
};

struct Rect2D
{
	float x;
	float y;
	float width;
	float height;

	Rect2D(float x_pos, float y_pos, float width, float height)
	{
		x = x_pos;
		y = y_pos;
		this->width = width;
		this->height = height;
	}
};