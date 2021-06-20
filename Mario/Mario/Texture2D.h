#pragma once
#ifndef _TEXTURE2D_H
#define _TEXTURE2D_H
#include <SDL.h>
#include <string>
#include "constants.h"
#include "Commons.h"
#include <SDL_image.h>
#include <iostream>
class Texture2D
{

public:
	Texture2D(SDL_Renderer* renderer);
	~Texture2D();

	bool LoadFromFile(std::string path);
	void Free();
	void Render(Vector2D new_position, SDL_RendererFlip flip, double angle = 0.0);
	void Render(SDL_Rect src_rect, SDL_Rect src_dest, SDL_RendererFlip flip, double angle = 0.0);
	void Render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip);

	int GetWidth()
	{	
		return m_width;
	}
	int GetHeight()
	{
		return m_height;
	}
		
private:
	SDL_Texture* m_texture;
	SDL_Renderer* m_renderer;

	int m_width;
	int m_height;

};
#endif