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
}

void GameScreenLevel1::Render()
{
	//allows background to draw 
	m_background_texture->Render(Vector2D(), SDL_FLIP_NONE);
}
void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{

}

bool GameScreenLevel1::SetUpLevel()
{
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/BackgroundMB.png"))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
		return false;
	}
}
