#include "GameScreenLevel1.h"
#include <fstream>
#include "Camera.h"

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	//sets up music 
	LoadMusic("Music/Mario.mp3");
	Mix_PlayMusic(g_music, -1);
	//render & lvl set
	m_renderer = renderer;
	m_level_map = nullptr;
	SetLevelMap();
	SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1()
{
	//map
	delete m_background_texture;
	m_background_texture = nullptr;
	delete m_level_map;
	m_level_map = nullptr;
	//players
	delete Mario;
	Mario = nullptr;
	delete Luigi;
	Luigi = nullptr;
	//blocks
	delete m_pow_block;
	m_pow_block = nullptr;
	//enimies
	m_enemies.clear();
	m_coin.clear();
	m_goombas.clear();
	//audio
	Mix_FreeChunk(coinSound);
	coinSound = nullptr;
	delete coinSound;
}

void GameScreenLevel1::Render()
{
	//renders game assets
	m_background_texture->Render(Vector2D(-100 - Camera::GetInstance()->GetPosition().x, m_background_yPos), SDL_FLIP_NONE);

	// Render all tiles
	tileMap->DrawTileMap();

	Mario->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
	Luigi->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
	for (int i = 0; i < m_goombas.size(); i++)
	{
		m_goombas[i]->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
	}
	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);

	}
	for (int i = 0; i < m_coin.size(); i++)
	{
		m_coin[i]->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
	}

	// handles rendering text 
	Retro = TTF_OpenFont("Retro.ttf", 20);
	SDL_Surface* Scoresurf = TTF_RenderText_Solid(Retro,(std::string("Score: ") + to_string(Mario->Score)).c_str(), { 0,0,0 });
	SDL_Texture* ScoreTex = SDL_CreateTextureFromSurface(m_renderer, Scoresurf);
	scoreRect.x = 0.0f;
	scoreRect.y = 0.0f;
	SDL_RenderCopy(m_renderer, ScoreTex, NULL, &scoreRect);
	SDL_QueryTexture(ScoreTex, NULL, NULL, &scoreRect.w, &scoreRect.h);

	SDL_Surface* Lives = TTF_RenderText_Solid(Retro, (std::string("Lives: ") + to_string(Mario->lifeCount)).c_str(), { 0,0,0 });
	SDL_Texture* LivesTex = SDL_CreateTextureFromSurface(m_renderer, Lives);
	textRect.x = 550.0f;
	textRect.y = 0.0f;
	//clears the texts memory 
	TTF_CloseFont(Retro);
	SDL_RenderCopy(m_renderer, LivesTex, NULL, &textRect);
	SDL_QueryTexture(LivesTex, NULL, NULL, &textRect.w, &textRect.h);
	SDL_FreeSurface(Lives);
	SDL_DestroyTexture(LivesTex);
	SDL_FreeSurface(Scoresurf);
	SDL_DestroyTexture(ScoreTex);
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	cout << m_level_map->GetTileAt(Mario->foot_position + 1.0f, Mario->centralX_position) << endl;
	//player
	Mario->Update(deltaTime, e);
	Luigi->Update(deltaTime, e);
	//enimes
	UpdateEnemies(deltaTime, e);
	UpdateGoomba(deltaTime, e);
	UpdateCoin(deltaTime, e);
	UpdateLuckyBlock(deltaTime, e);
	HandleViewportCollision();
	UpdateCameraPosition();
	CheckWin();
	OnMapCheck(deltaTime, e);

	// pow block screen shake
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

	//resets lvl if there are no lives left
	if (Mario->lifeCount == 0)
	{
		screen = SCREEN_GAMEOVER;
	}
	//handles the shell collision
	for (unsigned int i = 0; i < m_enemies.size(); i++)
	{
		for (unsigned int j = 0; j < m_goombas.size(); j++)
		{
			if (m_enemies[i]->GetInjured() && Collisions::Instance()->Circle(m_enemies[i], m_goombas[j]))
			{
				Mario->Score = Mario->Score + 200;
				m_goombas[j]->SetAlive(false);
				Mix_PlayChannel(-1, stompSound, 0);
				std::cout << Mario->Score << endl;
			}
		}
		for (unsigned int j = 0; j < m_enemies.size(); j++)
		{
			if (m_enemies[i]->GetInjured() && !m_enemies[j]->GetInjured() && Collisions::Instance()->Circle(m_enemies[i], m_enemies[j]))
			{
					Mario->Score = Mario->Score + 200;
					m_enemies[j]->SetAlive(false);
					Mix_PlayChannel(-1, stompSound, 0);
					std::cout << Mario->Score << endl;
			}
		}
	}
}

bool GameScreenLevel1::SetUpLevel()
{
	//loads in background 
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/Background.png"))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
		return false;
	}
	//payers
	Mario = new CharacterMario(m_renderer, "Images/MarioSheet.png", Vector2D(50, 342), m_level_map);
	Luigi = new CharacterLuigi(m_renderer, "Images/LuigiSheet.png", Vector2D(50, 342), m_level_map);

	SetUpTileMap();

	//inital enemies
	//CreateKoopa(Vector2D(1708, 102), FACING_RIGHT, KOOPA_SPEED); 
	//CreateKoopa(Vector2D(2370, -402), FACING_RIGHT, KOOPA_SPEED);
	CreateGoomba(Vector2D(480, 354), FACING_LEFT, 45.0f);
	CreateGoomba(Vector2D(1429, 354), FACING_LEFT, 45.0f);
	CreateGoomba(Vector2D(1488, 354), FACING_LEFT, 45.0f);
	CreateGoomba(Vector2D(3700, 354), FACING_LEFT, 45.0f);
	CreateGoomba(Vector2D(3773, 354), FACING_LEFT, 45.0f);
	CreateKoopa(Vector2D(3600, 354), FACING_LEFT, KOOPA_SPEED);
	CreateGoomba(Vector2D(5200, 354), FACING_LEFT, 45.0f);
	CreateGoomba(Vector2D(5270, 354), FACING_LEFT, 45.0f);
	CreateKoopa(Vector2D(5270, 354), FACING_LEFT, KOOPA_SPEED);
	

	
	//screen shake 
	m_screenshake = false;
	m_background_yPos = 0.0f;
}

void GameScreenLevel1::SetLevelMap()
{
	//sets up lvl map
	int map[MAP_HEIGHT][MAP_WIDTH] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };

	if (m_level_map != nullptr)
	{
		delete m_level_map;
	}
	m_level_map = new LevelMap(map);
}

void GameScreenLevel1::UpdateEnemies(float deltaTime, SDL_Event e)
{
	if (!m_enemies.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < m_enemies.size(); i++)
		{
			//check if the enemy is on the bottom row of tiles
			if (m_enemies[i]->GetPosition().y > 300.0f)
			{
				//allows player to kill enimies on the bottom row
				Mario->foot_position = Mario->foot_position - 0.0001f;
				Luigi->foot_position = Luigi->foot_position - 0.0001f;
			}
			m_enemies[i]->Update(deltaTime, e);
			//allows enemy to take damage when jumped on top of
			if (!m_enemies[i]->GetInjured())
			{
				if (Mario->foot_position < m_enemies[i]->foot_position && Collisions::Instance()->Circle(m_enemies[i], Mario))
				{
					Mario->hop();
					m_enemies[i]->TakeDamage();
				}
				else if (Luigi->foot_position < m_enemies[i]->foot_position && Collisions::Instance()->Circle(m_enemies[i], Luigi))
				{
					Luigi->hop();
					m_enemies[i]->TakeDamage();
				}
			}
			//player collision after being damaged 
			if (Collisions::Instance()->Box(Mario->GetCollisionBox(), m_enemies[i]->GetCollisionBox()))
			{
				if (m_enemies[i]->GetInjured())
				{
					m_enemies[i]->m_facing_direction = Mario->m_facing_direction;
					if (m_enemies[i]->m_facing_direction == FACING_RIGHT)
					{
						m_enemies[i]->m_moving_right = true;
						m_enemies[i]->m_moving_left = false;
					}
					if (m_enemies[i]->m_facing_direction == FACING_LEFT)
					{
						m_enemies[i]->m_moving_right = false;
						m_enemies[i]->m_moving_left = true;
					}
					//kills enemey
					if (Mario->foot_position < m_enemies[i]->foot_position && Collisions::Instance()->Circle(m_enemies[i], Mario))
					{
						//kills enemey
						Mario->Score = Mario->Score + 200;
						m_enemies[i]->SetAlive(false);
						std::cout << Mario->Score << endl;
					}
				}
				else
				{
					//kill Mario
					Mario->Dead(deltaTime);
					Mario->lifeCount = Mario->lifeCount - 1;
				}
			}
			else if (Collisions::Instance()->Box(Luigi->GetCollisionBox(), m_enemies[i]->GetCollisionBox()))
			{
				if (m_enemies[i]->GetInjured())
				{
					m_enemies[i]->m_facing_direction = Luigi->m_facing_direction;
					if (m_enemies[i]->m_facing_direction == FACING_RIGHT)
					{
						m_enemies[i]->m_moving_right = true;
						m_enemies[i]->m_moving_left = false;
					}
					if (m_enemies[i]->m_facing_direction == FACING_LEFT)
					{
						m_enemies[i]->m_moving_right = false;
						m_enemies[i]->m_moving_left = true;
					}
					//kills enemey
					if (Luigi->foot_position < m_enemies[i]->foot_position && Collisions::Instance()->Circle(m_enemies[i], Luigi))
					{
						//kills enemey
						Mario->Score = Mario->Score + 200;
						m_enemies[i]->SetAlive(false);
						std::cout << Mario->Score << endl;
					}
				}
				else
				{
					//kill Luigi
					Mario->lifeCount = Mario->lifeCount - 1;
					Luigi->Dead(deltaTime);
				}
			}

			//if no longer alive then delete
			if (!m_enemies[i]->GetAlive())
			{
				std::cout << "ded" << endl;
				enemyIndexToDelete = i;
			}
		}
		if (enemyIndexToDelete != -1)
		{
			m_enemies.erase(m_enemies.begin() + enemyIndexToDelete);
		}
	}
}

void GameScreenLevel1::UpdateGoomba(float deltaTime, SDL_Event e)
{
	if (!m_goombas.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < m_goombas.size(); i++)
		{
			if (m_goombas[i]->GetPosition().y > 300.0f)
			{
				//allows player to kill enimies on the bottom row

			}
			m_goombas[i]->Update(deltaTime, e);
			{
				if (!m_goombas[i]->GetInjured())
				{
					//checks if player collides with enemy then decides what to do 
					//mario
					if (Collisions::Instance()->Circle(Mario, m_goombas[i]) && Mario->foot_position < m_goombas[i]->head_position)
					{
						//kill enemy 
						Mario->hop();
						m_goombas[i]->TakeDamage();
						if (m_goombas[i]->GetInjured())
						{
							cout << m_goombas[i]->GetPosition().y << endl;
							cout << Mario->foot_position << endl;
							cout << m_goombas[i]->head_position << endl;
							Mario->Score = Mario->Score + 200;
							m_goombas[i]->SetAlive(false);
							std::cout << Mario->Score << endl;
						}
					}
					//side collision
					if (Collisions::Instance()->Box(Mario->GetCollisionBox(), m_goombas[i]->GetCollisionBox()) && Mario->foot_position <= m_goombas[i]->head_position)
					{
						cout << Mario->foot_position << endl;
						cout << m_goombas[i]->head_position << endl;
						//kill mario
						Mario->Dead(deltaTime);
						Mario->lifeCount = Mario->lifeCount - 1;
					}
					//luigi
					if (Collisions::Instance()->Circle(Luigi, m_goombas[i]) && Luigi->foot_position < m_goombas[i]->head_position)
					{
						//kill enemy 
						Luigi->hop();
						m_goombas[i]->TakeDamage();
						if (m_goombas[i]->GetInjured())
						{
							cout << Luigi->foot_position << endl;
							cout << m_goombas[i]->head_position << endl;
							Mario->Score = Mario->Score + 200;
							m_goombas[i]->SetAlive(false);
							std::cout << Mario->Score << endl;
						}
					}
					//side collision
					if (Collisions::Instance()->Box(Luigi->GetCollisionBox(), m_goombas[i]->GetCollisionBox()) && Luigi->foot_position <= m_goombas[i]->head_position)
					{
						cout << Luigi->foot_position << endl;
						cout << m_goombas[i]->head_position << endl;
						//kill mario
						Luigi->Dead(deltaTime);
						Mario->lifeCount = Mario->lifeCount - 1;
					}
				}
			}
				//delete dead enemy
				if (!m_goombas[i]->GetAlive())
				{
					std::cout << "ded" << endl;
					enemyIndexToDelete = i;
				}
				}
			if (enemyIndexToDelete != -1)
			{
				m_goombas.erase(m_goombas.begin() + enemyIndexToDelete);
			}
	}
}

void GameScreenLevel1::UpdateCoin(float deltaTime, SDL_Event e)
{
	//checks if there is any coins 
	if (!m_coin.empty())
	{
		int coinIndexToDelete = -1;
		for (unsigned int i = 0; i < m_coin.size(); i++)
		{
			//checks if coin and player collide  
			m_coin[i]->Update(deltaTime, e);
			if (Collisions::Instance()->Circle(m_coin[i], Luigi) || (Collisions::Instance()->Circle(m_coin[i], Mario)))
			{
				Mario->Score = Mario->Score + 100;
				m_coin[i]->SetAlive(false);
				Mix_PlayChannel(-1, coinSound, 0);
				std::cout << Mario->Score << endl;
				if (!m_coin[i]->GetAlive())
				{
					std::cout << "Collected" << endl;
					coinIndexToDelete = i;
				}
				//removes collected coins
				if (coinIndexToDelete != -1)
				{
					m_coin.erase(m_coin.begin() + coinIndexToDelete);
				}
			}
		}
	}
}

void GameScreenLevel1::SortScore()
{
	std::ifstream indata("HighScores.txt");

	indata >> highScore;
	indata.close();

	if (Mario->Score > highScore)
	{
		std::ofstream outfile;
		outfile.open("HighScores.txt");

		if (!outfile.is_open())
		{
			sdt:cout << "save file could not be opened" << endl;
			std::exit(1);
		}
		outfile << Mario->Score << std::endl;
		outfile.close();
	}
}

void GameScreenLevel1::DoScreenshake() 
{
	//screen shake when pow block is hit 
	m_screenshake = true;
	m_shake_time = SHAKE_DURATION;
	m_wobble = 0.0f;

	for (unsigned int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->TakeDamage();
	}
	for (unsigned int i = 0; i < m_goombas.size(); i++)
	{
		m_goombas[i]->Jump();
		Mario->Score = Mario->Score + 200;
		std::cout << Mario->Score << endl;
		m_goombas[i]->SetAlive(false);
	}
}


void GameScreenLevel1::HandleViewportCollision()
{
	if (Mario->m_alive)
	{
		if (Mario->GetPosition().x <= Camera::GetInstance()->GetCameraBounds().x)
		{
			Mario->SetPosition(Vector2D(Mario->GetPosition().x + 1, Mario->GetPosition().y));
		}
	}
	else if (Luigi->m_alive)
	{
		if (Luigi->GetPosition().x <= Camera::GetInstance()->GetCameraBounds().x)
		{
			Luigi->SetPosition(Vector2D(Luigi->GetPosition().x + 1, Luigi->GetPosition().y));
		}
	}
}

void GameScreenLevel1::UpdateCameraPosition()
{
	if (Mario->m_alive)
	{
		if ((Mario->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2 > mLastCamXPos)
		{
			Camera::GetInstance()->SetPosition(Vector2D((Mario->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2, m_background_yPos));
			mLastCamXPos = (Mario->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2;
		}
	}
	else if (Luigi->m_alive)
	{
		if (Luigi->GetPosition().x <= Camera::GetInstance()->GetCameraBounds().x)
		{
			Camera::GetInstance()->SetPosition(Vector2D((Luigi->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2, m_background_yPos));
			mLastCamXPos = (Luigi->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2;
		}

		if ((Luigi->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2 > mLastCamXPos)
		{
			Camera::GetInstance()->SetPosition(Vector2D((Luigi->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2, m_background_yPos));
			mLastCamXPos = (Luigi->GetPosition().x + 32 / 2) - CAMERA_WIDTH / 2;
		}
	}
}

void GameScreenLevel1::SetUpTileMap()
{
	// Read file
	std::ifstream file("Maps/level1.txt");

	// Get column length
	int rows = 0;
	int columns = 0;
	std::string line;
	if (file.is_open())
	{
		while (std::getline(file, line))
			rows++;
	}

	// Assuming the width is fixed, get column count
	for (int x = 0; x < line.length(); x++)
	{
		if (line[x] != ' ')
			columns++;
	}

	char** map;
	map = new char* [rows];

	// Predefine map
	for (unsigned int i = 0; i < rows; i++)
		map[i] = new char[columns];

	// Hop back to the beginning of the file
	file.clear();
	file.seekg(0, std::ios::beg);

	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			file >> map[row][column];
		}
	}

	file.close();

	// Create new TileMap
	tileMap = new TileMap(m_renderer);
	tileMap->GenerateTileMap(map, rows, columns);
}

void GameScreenLevel1::UpdateLuckyBlock(float deltaTime, SDL_Event e)
{
	if (tileMap->GetTileAt(Mario->centralX_position, Mario->head_position) != nullptr)
	{
		if (tileMap->GetTileAt(Mario->centralX_position, Mario->head_position)->GetBlock() != nullptr)
		{
			if (tileMap->GetTileAt(Mario->centralX_position, Mario->head_position)->GetBlock()->GetBlockType() == Blocks::BlockType::BLOCK_LUCKY)
			{
				if (tileMap->GetTileAt(Mario->centralX_position, Mario->head_position)->GetBlock()->IsAvailable())
				{
					// Collision, spawn coin and set question mark block to unavailable
					tileMap->GetTileAt(Mario->centralX_position, Mario->head_position)->GetBlock()->SetAvailable(false);
					CreateCoin(Vector2D(tileMap->GetTileAt(Mario->centralX_position, Mario->head_position)->GetBlock()->GetPosition().x + 3, tileMap->GetTileAt(Mario->centralX_position, Mario->head_position)->GetBlock()->GetPosition().y - 32));
				}
			}
		}
	}

	if (tileMap->GetTileAt(Luigi->centralX_position, Luigi->head_position) != nullptr)
	{
		if (tileMap->GetTileAt(Luigi->centralX_position, Luigi->head_position)->GetBlock() != nullptr)
		{
			if (tileMap->GetTileAt(Luigi->centralX_position, Luigi->head_position)->GetBlock()->GetBlockType() == Blocks::BlockType::BLOCK_LUCKY)
			{
				if (tileMap->GetTileAt(Luigi->centralX_position, Luigi->head_position)->GetBlock()->IsAvailable())
				{
					// Collision, spawn coin and set question mark block to unavailable
					tileMap->GetTileAt(Luigi->centralX_position, Luigi->head_position)->GetBlock()->SetAvailable(false);
					CreateCoin(Vector2D(tileMap->GetTileAt(Luigi->centralX_position, Luigi->head_position)->GetBlock()->GetPosition().x + 3, tileMap->GetTileAt(Luigi->centralX_position, Luigi->head_position)->GetBlock()->GetPosition().y - 32));
				}
			}
		}
	}
}

void GameScreenLevel1::OnMapCheck(float deltaTime, SDL_Event e)
{
	if (m_level_map->GetTileAt(Mario->foot_position + 1.0f, Mario->centralX_position) == 4)
	{
		Mario->Dead(deltaTime);
		Mario->lifeCount = Mario->lifeCount - 1;
	}

	if (m_level_map->GetTileAt(Luigi->foot_position + 1.0f, Luigi->centralX_position) == 4)
	{
		Luigi->Dead(deltaTime);
		Mario->lifeCount = Mario->lifeCount - 1;
	}
}

void GameScreenLevel1::CheckWin()
{
	if (m_level_map->GetTileAt(Mario->centralYPositionInGrid, Mario->centralX_position) == 3)
	{
		SortScore();
		screen = SCREEN_LEVEL2;
	}

	if (m_level_map->GetTileAt(Luigi->centralYPositionInGrid, Luigi->centralX_position) == 3)
	{
		SortScore();
		screen = SCREEN_LEVEL2;
	}
}

void GameScreenLevel1::CreateKoopa(Vector2D position, FACING direction, float speed)
{
	Koopa* koopa = new Koopa(m_renderer, "Images/Koopa.png", m_level_map, position, direction, speed);
	m_enemies.push_back(koopa);
}

void GameScreenLevel1::CreateCoin(Vector2D position)
{
	Coin* coin = new Coin(m_renderer, "Images/Coin.png", m_level_map, position);
	m_coin.push_back(coin);
}

void GameScreenLevel1::CreateGoomba(Vector2D position, FACING direction, float speed)
{
	Goomba* goomba = new Goomba(m_renderer, "Images/Goomba.png", m_level_map, position, direction, speed);
	m_goombas.push_back(goomba);
}

void GameScreenLevel1::LoadMusic(string path)
{
	//loads music 
	g_music = Mix_LoadMUS(path.c_str());
	if (g_music == nullptr)
	{
		cout << "Failed to load music. Error: " << Mix_GetError() << endl;
	}
}