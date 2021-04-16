#include "GameOver.h"
#include "Texture2D.h"
#include <iostream>
#include <sstream>
#include <string>

GameOver::GameOver(SDL_Renderer* renderer) : GameScreen(renderer)
{
	m_renderer = renderer;
	SetUpLevel();
}

GameOver::~GameOver()
{
	delete m_background_texture;
	m_background_texture = nullptr;
}

void GameOver::Render()
{
	m_background_texture->Render(Vector2D(0, 0), SDL_FLIP_NONE);

	Retro = TTF_OpenFont("Retro.ttf", 30);
	SDL_Surface* text = TTF_RenderText_Solid(Retro, "GAME OVER", { 255,255,255 });
	SDL_Texture* textTex = SDL_CreateTextureFromSurface(m_renderer, text);
	GameOverRect.x = 415.0f;
	GameOverRect.y = 200.0f;
	TTF_CloseFont(Retro);
	SDL_RenderCopy(m_renderer, textTex, NULL, &GameOverRect);
	SDL_QueryTexture(textTex, NULL, NULL, &GameOverRect.w, &GameOverRect.h);
	SDL_FreeSurface(text);
	SDL_DestroyTexture(textTex);

	Retro = TTF_OpenFont("Retro.ttf", 20);
	SDL_Surface* Tut = TTF_RenderText_Solid(Retro, "1 SCORES", { 255,255,255 });
	SDL_Texture* TutTex = SDL_CreateTextureFromSurface(m_renderer, Tut);
	ScoreRect.x = 440.0f;
	ScoreRect.y = 250.0f;
	TTF_CloseFont(Retro);
	SDL_RenderCopy(m_renderer, TutTex, NULL, &ScoreRect);
	SDL_QueryTexture(TutTex, NULL, NULL, &ScoreRect.w, &ScoreRect.h);
	SDL_FreeSurface(Tut);
	SDL_DestroyTexture(TutTex);

	Retro = TTF_OpenFont("Retro.ttf", 20);
	SDL_Surface* Exit = TTF_RenderText_Solid(Retro, "2 PLAY AGAIN", { 255,255,255 });
	SDL_Texture* ExitTex = SDL_CreateTextureFromSurface(m_renderer, Exit);
	PlayRect.x = 425.0f;
	PlayRect.y = 300.0f;
	TTF_CloseFont(Retro);
	SDL_RenderCopy(m_renderer, ExitTex, NULL, &PlayRect);
	SDL_QueryTexture(ExitTex, NULL, NULL, &PlayRect.w, &PlayRect.h);
	SDL_FreeSurface(Exit);
	SDL_DestroyTexture(ExitTex);
}

void GameOver::Update(float deltaTime, SDL_Event e)
{
	switch (e.type) {
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_2:
			screen = SCREEN_LEVEL1;
			break;
		default:
			break;
		}
	}
}

bool GameOver::SetUpLevel()
{
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/GameOver.png"))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
		return false;
	}
}

