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
	delete my_character;
	my_character = nullptr;
}

void GameScreenLevel1::Render()
{
	//allows background to draw 
	m_background_texture->Render(Vector2D(), SDL_FLIP_NONE);
	my_character->Render();
}
void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	my_character->Update(deltaTime, e);
}

bool GameScreenLevel1::SetUpLevel()
{
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/BackgroundMB.png"))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
		return false;
	}
	my_character = new Character(m_renderer, "Images/Mario.png", Vector2D(64, 330));
}
