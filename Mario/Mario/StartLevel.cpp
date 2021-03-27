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
}
void StartLevel::Update(float deltaTime, SDL_Event e)
{
	switch (e.type) {
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_SPACE:
			game_screen_manager->ChangeScreen(SCREEN_LEVEL1);
			break;
		default:
			break;
		}
	}

}

bool StartLevel::SetUpLevel()
{
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/BackgroundMB.png"))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
		return false;
	}
}
