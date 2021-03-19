#include "Texture2D.h"
#include "StartScreen.h"
#include <iostream>

StartScreen::StartScreen(SDL_Renderer* renderer) : GameScreen(renderer)
{
	m_renderer = renderer;

}

StartScreen::~StartScreen()
{
	delete m_background_texture;
	m_background_texture = nullptr;
}

void StartScreen::Render()
{

	//calls the class render function
	m_background_texture->Render(Vector2D(0,0), SDL_FLIP_NONE);
}

void StartScreen::Update(float deltaTime, SDL_Event e)
{

}


bool StartScreen::SetUpLevel()
{
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/BackgroundMB.png"))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
		return false;
	}

}