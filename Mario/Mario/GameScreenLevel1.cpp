#include "Texture2D.h"
#include "GameScreenLevel1.h"
#include <iostream>

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	m_renderer = renderer;
	SetUpLevel();

}
GameScreenLevel1::~GameScreenLevel1()
{
	delete m_background_texture;
	m_background_texture = nullptr;
	delete Mario;
	Mario = nullptr;
}

void GameScreenLevel1::Render()
{
	//allows background to draw 
	m_background_texture->Render(Vector2D(), SDL_FLIP_NONE);
	Mario->Render();
	Luigi->Render();
}
void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	Mario->Update(deltaTime, e);
	Luigi->Update(deltaTime, e);
}

bool GameScreenLevel1::SetUpLevel()
{
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/BackgroundMB.png"))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
		return false;
	}
	Mario = new CharacterMario(m_renderer, "Images/Mario.png", Vector2D(64, 330));
	Luigi = new CharacterLuigi(m_renderer, "Images/Luigi.png", Vector2D(64, 330));
}
