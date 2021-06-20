#include "ControlsScreen.h"

ControlsScreen::ControlsScreen(SDL_Renderer* renderer) : GameScreen(renderer)
{
	m_renderer = renderer;
	SetUpLevel();
}

ControlsScreen::~ControlsScreen()
{
	delete m_background_texture;
	m_background_texture = nullptr;
	
}

void ControlsScreen::Render()
{
	//redners background 
	m_background_texture->Render(Vector2D(0, 0), SDL_FLIP_NONE);
}

void ControlsScreen::Update(float deltaTime, SDL_Event e)
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

bool ControlsScreen::SetUpLevel()
{
	//loads in the background texture and error checks it 
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/Controls.png"))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
		return false;
	}
}