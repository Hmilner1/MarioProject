#include "Texture2D.h"
#include "StartLevel.h"
#include <iostream>

StartLevel::StartLevel(SDL_Renderer* renderer) : GameScreen(renderer)
{
	m_renderer = renderer;
	SetUpLevel();



}

StartLevel::~StartLevel()
{
	delete m_background_texture;
	m_background_texture = nullptr;
}

void StartLevel::Render()
{
	m_background_texture->Render(Vector2D(0, 0), SDL_FLIP_NONE);
	
	Retro = TTF_OpenFont("Retro.ttf", 20);
	SDL_Surface* text = TTF_RenderText_Solid(Retro, "Enter To Stat", { 0,0,0 });
	SDL_Texture* textTex = SDL_CreateTextureFromSurface(m_renderer, text);
	textRect.x = 170.0f;
	textRect.y = 130.0f;
	TTF_CloseFont(Retro);
	SDL_RenderCopy(m_renderer, textTex, NULL, &textRect);
	SDL_QueryTexture(textTex, NULL, NULL, &textRect.w, &textRect.h);
	SDL_FreeSurface(text);
	SDL_DestroyTexture(textTex);

	Retro = TTF_OpenFont("Retro.ttf", 20);
	SDL_Surface* Exit = TTF_RenderText_Solid(Retro, "Escape To Exit", { 0,0,0 });
	SDL_Texture* ExitTex = SDL_CreateTextureFromSurface(m_renderer, Exit);
	textRect.x = 170.0f;
	textRect.y = 200.0f;
	TTF_CloseFont(Retro);
	SDL_RenderCopy(m_renderer, ExitTex, NULL, &textRect);
	SDL_QueryTexture(ExitTex, NULL, NULL, &textRect.w, &textRect.h);
	SDL_FreeSurface(Exit);
	SDL_DestroyTexture(ExitTex);
}

void StartLevel::Update(float deltaTime, SDL_Event e)
{
	switch (e.type) {
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_RETURN:
			screen = SCREEN_LEVEL1;
			break;
		default:
			break;
		}
	}
}

bool StartLevel::SetUpLevel()
{
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/Start2.png"))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
		return false;
	}
}
