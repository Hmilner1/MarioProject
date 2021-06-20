#include "ScoreScreen.h"
#include <fstream>


ScoreScreen::ScoreScreen(SDL_Renderer* renderer) : GameScreen(renderer)
{
	m_renderer = renderer;
	SetUpLevel();
}

ScoreScreen::~ScoreScreen()
{
	delete m_background_texture;
	m_background_texture = nullptr;
}

void ScoreScreen::Render()
{
	//redners background 
	m_background_texture->Render(Vector2D(0, 0), SDL_FLIP_NONE);

	Retro = TTF_OpenFont("Retro.ttf", 40);
	SDL_Surface* Scoresurf = TTF_RenderText_Solid(Retro, (std::string("Score: ") + to_string(hightScore)).c_str(), { 255,255,255 });
	SDL_Texture* ScoreTex = SDL_CreateTextureFromSurface(m_renderer, Scoresurf);
	textRect.x = 210.0f;
	textRect.y = 200.0f;
	SDL_RenderCopy(m_renderer, ScoreTex, NULL, &textRect);
	SDL_QueryTexture(ScoreTex, NULL, NULL, &textRect.w, &textRect.h);
	SDL_FreeSurface(Scoresurf);
	SDL_DestroyTexture(ScoreTex);

}

void ScoreScreen::Update(float deltaTime, SDL_Event e)
{
	switch (e.type) {
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_RETURN:
			screen = SCREEN_LEVEL1;
			break;
		}
	}
}

bool ScoreScreen::SetUpLevel()
{
	//loads in the background texture and error checks it 
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/GameOver.png"))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
		return false;
	}

	std::ifstream indata("HighScores.txt");

	indata >> hightScore;
	indata.close();
	

}