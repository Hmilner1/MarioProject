#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "constants.h"
using namespace std;
SDL_Window* g_window = nullptr;

bool InitSDL();
void CLoseSDL();
int main(int argc, char* args[])
{
	if (InitSDL())
	{
		SDL_Delay(5000);
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
}
void CLoseSDL()
{
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	IMG_Quit();
	SDL_Quit();
}
