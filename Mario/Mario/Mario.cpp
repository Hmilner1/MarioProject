#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "constants.h"
using namespace std;
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
SDL_Texture* g_texture = nullptr;

bool InitSDL();
void CLoseSDL();
bool Update();
void Render();
SDL_Texture* LoadTextureFromFile(string path);
void FreeTexture();
int main(int argc, char* args[])
{
	if (InitSDL())
	{
		bool quit = false;

		while (!quit)
		{
			quit = Update();
		}
	}
	CLoseSDL();
	return 0;
}

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//sets up SDL
		cout << "SDL did not initialise. Error: " << SDL_GetError();
		return false; 
	}
	else
	{
		//creates window
		g_window = SDL_CreateWindow("Games Engine Creation",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
		if (g_window == nullptr)
		{
			cout << "Window was not created. Error: " << SDL_GetError();
			return false;
		}
	}
	return true;
	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
	if (g_renderer != nullptr)
	{
		int imageFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imageFlags) & imageFlags))
		{
			cout << "SDL_Image could not initialise. Error: " << IMG_GetError();
			return false;
		}
	}
	else
	{
		cout << "Render could not initilise. Error: " << SDL_GetError();
		return false;
	}
}
void CLoseSDL()
{
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	IMG_Quit();
	SDL_Quit();
}
bool Update()
{
	//event handler 
	SDL_Event e;

	//see the events that have happened 
	SDL_PollEvent(&e);

	switch (e.type)
	{
	case SDL_QUIT:
		return true;
		break;
	
	case SDL_KEYUP:
	{
		switch (e.key.keysym.sym)
	case SDLK_q:
		return true;
		break;
	}
	}
	return false;
}
void Render()
{
	SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(g_renderer);
	SDL_Rect renderLocation = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	SDL_RenderCopyEx(g_renderer, g_texture, NULL, &renderLocation, 0, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(g_renderer);
}
SDL_Texture* LoadTextureFromFile(string path)
{
	SDL_Texture* p_texture = nullptr;
	SDL_Surface* p_surface = IMG_Load(path.c_str());
	if (p_surface != nullptr)
	{
		//create the texture from the pixels on the surface
		p_texture = SDL_CreateTextureFromSurface(g_renderer, p_surface);
		if (p_texture == nullptr)
		{
			cout << "Unable to create texture from surface. Error: " << SDL_GetError();
		}
		//remove the loaded surface now that we have a texture
		SDL_FreeSurface(p_surface);
	}
	else
	{
		cout << "Unable to create texture from surface. Error: " << IMG_GetError();
	}
	return p_texture;
}
