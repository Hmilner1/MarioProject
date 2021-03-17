#include "Texture2D.h"
#include "GameScreenLevel1.h"
#include <iostream>

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	m_renderer = renderer;
	m_level_map = nullptr;
	SetLevelMap();
	SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1()
{
	delete m_background_texture;
	m_background_texture = nullptr;
	delete Mario;
	Mario = nullptr;
	Luigi = nullptr;
	delete Luigi;
	delete m_pow_block;
	m_pow_block = nullptr;
}

void GameScreenLevel1::Render()
{
	//calls the class render function
	m_background_texture->Render(Vector2D(0, m_background_yPos), SDL_FLIP_NONE);
	Luigi->Render();
	Mario->Render();
	m_pow_block->Render();
}
void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	Mario->Update(deltaTime, e);
	Luigi->Update(deltaTime, e);
	UpdatePowBlock();
	
	if (Collisions::Instance()->Box(Mario->GetCollisionBox(), Luigi->GetCollisionBox()))
	{
		//cout << "Box hit" << endl;
	}

	if (m_screenshake)
	{
		m_shake_time -= deltaTime;
		m_wobble++;
		m_background_yPos = sin(m_wobble);
		m_background_yPos *= 3.0f;
		//end shake after duration
		if (m_shake_time <= 0.0f)
		{
			m_shake_time = false;
			m_background_yPos = 0.0f;
		}
	}
}

bool GameScreenLevel1::SetUpLevel()
{
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/BackgroundMB.png"))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
		return false;
	}
	//Mario = new CharacterMario(m_renderer, "Images/Mario.png", Vector2D(64, 330));
	Mario = new CharacterMario(m_renderer, "Images/Mario.png", Vector2D(64, 330), m_level_map);
	//Luigi = new CharacterLuigi(m_renderer, "Images/Luigi.png", Vector2D(64, 330));
	Luigi = new CharacterLuigi(m_renderer, "Images/Luigi.png", Vector2D(64, 330), m_level_map);
	m_pow_block = new PowBlock(m_renderer, m_level_map);
	m_screenshake = false;
	m_background_yPos = 0.0f;
}

void GameScreenLevel1::SetLevelMap()
{
	int map[MAP_HEIGHT][MAP_WIDTH] = { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0 },
									 { 1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0 },
									 { 1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };
	//clear any old maps
	if (m_level_map != nullptr)
	{
		delete m_level_map;
	}
	//set the new one
	m_level_map = new LevelMap(map);
}

void GameScreenLevel1::UpdatePowBlock()
{

	if (Collisions::Instance()->Box(Mario->GetCollisionBox(), m_pow_block->GetCollisionBox()) && m_pow_block->IsAvailable())
	{
		cout << "Box hit" << endl;

		//collided while jumping
		if (Mario->IsJumping())
		{
			DoScreenshake();
			m_pow_block->TakeHit();
			Mario->CancelJump();
		}
		
	}
}

void GameScreenLevel1::DoScreenshake()
{
	m_screenshake = true;
	m_shake_time = SHAKE_DURATION;
	m_wobble = 0.0f;
}