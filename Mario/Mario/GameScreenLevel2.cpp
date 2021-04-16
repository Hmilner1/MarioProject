#include "GameScreenLevel2.h"
#include <sstream>
#include <iostream>

GameScreenLevel2::GameScreenLevel2(SDL_Renderer* renderer) : GameScreen(renderer)
{
	LoadMusic("Music/MarioUnderworld.mp3");
	Mix_PlayMusic(g_music, -1);
	

	m_renderer = renderer;
	m_level_map = nullptr;
	SetLevelMap();
	SetUpLevel();
}

GameScreenLevel2::~GameScreenLevel2()
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
	//delete m_pow_block;
	//m_pow_block = nullptr;
	//enimies
	m_enemies.clear();
	m_coin.clear();
	m_goombas.clear();
	//audio
	Mix_FreeChunk(coinSound);
	coinSound = nullptr;
	delete coinSound;
}

void GameScreenLevel2::Render()
{
	m_background_texture->Render(Vector2D(0, m_background_yPos), SDL_FLIP_NONE);
	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->Render();
	}
	for (int i = 0; i < m_coin.size(); i++)
	{
		m_coin[i]->Render();
	}
	for (int i = 0; i < m_goombas.size(); i++)
	{
		m_goombas[i]->Render();
	}
	Mario->Render();
	Luigi->Render();

	// handles rendering text 
	Retro = TTF_OpenFont("Retro.ttf", 20);
	//font surface 
	SDL_Surface* Scoresurf = TTF_RenderText_Solid(Retro, (std::string("ScoreLvl2: ") + to_string(Mario->Score)).c_str(), { 255,255,255 });
	SDL_Texture* ScoreTex = SDL_CreateTextureFromSurface(m_renderer, Scoresurf);
	scoreRect.x = 0.0f;
	scoreRect.y = 0.0f;
	//renders font
	SDL_RenderCopy(m_renderer, ScoreTex, NULL, &scoreRect);
	SDL_QueryTexture(ScoreTex, NULL, NULL, &scoreRect.w, &scoreRect.h);

	SDL_Surface* Lives = TTF_RenderText_Solid(Retro, (std::string("Lives: ") + to_string(Mario->lifeCount)).c_str(), { 255,255,255 });
	SDL_Texture* LivesTex = SDL_CreateTextureFromSurface(m_renderer, Lives);
	textRect.x = 900.0f;
	textRect.y = 0.0f;
	TTF_CloseFont(Retro);
	SDL_RenderCopy(m_renderer, LivesTex, NULL, &textRect);
	SDL_QueryTexture(LivesTex, NULL, NULL, &textRect.w, &textRect.h);
	SDL_FreeSurface(Lives);
	SDL_DestroyTexture(LivesTex);
	SDL_FreeSurface(Scoresurf);
	SDL_DestroyTexture(ScoreTex);
}

void GameScreenLevel2::Update(float deltaTime, SDL_Event e)
{
	Mario->Update(deltaTime, e);
	Luigi->Update(deltaTime, e);
	for (int i = 0; i < m_goombas.size(); i++)
	{
		m_goombas[i]->Update(deltaTime, e);
	}
	UpdateEnemies(deltaTime, e);
	UpdateGoomba(deltaTime, e);
	UpdateCoin(deltaTime, e);

	//resets lvl if there are no lives left
	if (Mario->lifeCount <= 0)
	{
		screen = SCREEN_GAMEOVER;
	}

	k_respawn_time -= deltaTime;
	if (k_respawn_time <= 0.0f)
	{
		CreateKoopa(Vector2D(64, 340), FACING_RIGHT, KOOPA_SPEED);
		k_respawn_time = RESPAWN;
	}
	g_respawn_time -= deltaTime;
	if (g_respawn_time <= 0.0f)
	{
		CreateGoomba(Vector2D(64, 340), FACING_RIGHT, 20.0f);
		g_respawn_time = URESPAWN;
	}
}

bool GameScreenLevel2::SetUpLevel()
{
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/Lvl2Background.png"))
	{
		std::cout << "Failed to load backgorund texture!" << std::endl;
		return false;
	}
	//payers
	Mario = new CharacterMario(m_renderer, "Images/MarioSheet.png", Vector2D(990, -400), m_level_map);
	Luigi = new CharacterLuigi(m_renderer, "Images/LuigiSheet.png", Vector2D(990, -400), m_level_map);

	//enemies
	CreateKoopa(Vector2D(150, 32), FACING_RIGHT, KOOPA_SPEED);
	CreateKoopa(Vector2D(325, 32), FACING_LEFT, KOOPA_SPEED);
	CreateGoomba(Vector2D(400, 32), FACING_RIGHT, 20.0f);
	CreateGoomba(Vector2D(450, 32), FACING_RIGHT, 20.0f); 

	CreateCoin(Vector2D(226, 256));
	CreateCoin(Vector2D(194, 256));
	CreateCoin(Vector2D(162, 256));
	CreateCoin(Vector2D(290, 256));
	CreateCoin(Vector2D(321, 256));
	CreateCoin(Vector2D(354, 256));
}

void GameScreenLevel2::SetLevelMap()
{
	//sets up lvl map
	int map[MAP_HEIGHT][MAP_WIDTH] = { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
									 { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };

	if (m_level_map != nullptr)
	{
		delete m_level_map;
	}

	m_level_map = new LevelMap(map);
}

void GameScreenLevel2::UpdateEnemies(float deltaTime, SDL_Event e)
{
	if (!m_enemies.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < m_enemies.size(); i++)
		{
			//check if the enemy is on the bottom row of tiles
			if (m_enemies[i]->GetPosition().y > 300.0f)
			{
				Mario->foot_position = Mario->foot_position - 0.0001f;
				Luigi->foot_position = Luigi->foot_position - 0.0001f;
				//is the enemy off screen to the left / right?
				if (m_enemies[i]->GetPosition().x < (float)(-m_enemies[i]->GetCollisionBox().width * 0.5f) ||
					m_enemies[i]->GetPosition().x > SCREEN_WIDTH - (float)(m_enemies[i]->GetCollisionBox().width * 0.55f))m_enemies[i]->SetAlive(false);
			}

			m_enemies[i]->Update(deltaTime, e);
			if (m_enemies[i]->GetPosition().x >= 985.0f || m_enemies[i]->GetPosition().x <= 63.0f)
			{
				m_enemies[i]->Turn();
			}
			else
			{
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
				//handles player collision when not happening on top 
				if (Collisions::Instance()->Box(Mario->GetCollisionBox(), m_enemies[i]->GetCollisionBox()))
				{
					if (m_enemies[i]->GetInjured())
					{
						Mario->Score = Mario->Score + 200;
						m_enemies[i]->SetAlive(false);
						std::cout << Mario->Score << endl;
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
						Mario->Score = Mario->Score + 200;
						m_enemies[i]->SetAlive(false);
						std::cout << Mario->Score << endl;
					}
					else
					{
						//kill Luigi
						Luigi->Dead(deltaTime);
						Mario->lifeCount = Mario->lifeCount - 1;
					}
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

void GameScreenLevel2::UpdateGoomba(float deltaTime, SDL_Event e)
{
	if (!m_goombas.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < m_goombas.size(); i++)
		{
			//check if the enemy is on the bottom row of tiles
			if (m_goombas[i]->GetPosition().y > 300.0f)
			{
				Mario->foot_position = Mario->foot_position - 0.0001f;
				Luigi->foot_position = Luigi->foot_position - 0.0001f;

				if (m_goombas[i]->GetPosition().x < (float)(-m_goombas[i]->GetCollisionBox().width * 0.5f) || m_goombas[i]->GetPosition().x > SCREEN_WIDTH - (float)(m_goombas[i]->GetCollisionBox().width * 0.55f))m_goombas[i]->SetAlive(false);
			}
			m_goombas[i]->Update(deltaTime, e);
			if (m_goombas[i]->GetPosition().x >= 985.0f || m_goombas[i]->GetPosition().x <= 63.0f)
			{
				m_goombas[i]->Turn();
			}
			else
			{
				//jump collison 
				if (!m_goombas[i]->GetInjured())
				{
					if (Mario->foot_position < m_goombas[i]->foot_position && Collisions::Instance()->Circle(m_goombas[i], Mario))
					{
						Mario->hop();
						m_goombas[i]->TakeDamage();
						if (m_goombas[i]->GetInjured())
						{
							Mario->Score = Mario->Score + 200;
							m_goombas[i]->SetAlive(false);
							std::cout << Mario->Score << endl;
						}
					}
					else if (Luigi->foot_position < m_goombas[i]->foot_position && Collisions::Instance()->Circle(m_goombas[i], Luigi))
					{
						Luigi->hop();
						m_goombas[i]->TakeDamage();
						if (m_goombas[i]->GetInjured())
						{
							Mario->Score = Mario->Score + 200;
							m_goombas[i]->SetAlive(false);
							std::cout << Mario->Score << endl;
						}
					}
				}
				//side collision
				if (Collisions::Instance()->Box(Mario->GetCollisionBox(), m_goombas[i]->GetCollisionBox()))
				{

					//kill mario
					Mario->Dead(deltaTime);
					Mario->lifeCount = Mario->lifeCount - 1;
				}
				else if (Collisions::Instance()->Box(Luigi->GetCollisionBox(), m_goombas[i]->GetCollisionBox()))
				{


					//kill mario
					Luigi->Dead(deltaTime);
					Mario->lifeCount = Mario->lifeCount - 1;
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

void GameScreenLevel2::UpdateCoin(float deltaTime, SDL_Event e)
{
	if (!m_coin.empty())
	{
		int coinIndexToDelete = -1;
		for (unsigned int i = 0; i < m_coin.size(); i++)
		{
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
				//remove dead enemies -1 each update
				if (coinIndexToDelete != -1)
				{
					m_coin.erase(m_coin.begin() + coinIndexToDelete);
				}
			}
		}
	}
}


void GameScreenLevel2::CreateKoopa(Vector2D position, FACING direction, float speed)
{
	Koopa* koopa = new Koopa(m_renderer, "Images/UKoopa.png", m_level_map, position, direction, speed);
	m_enemies.push_back(koopa);
}

void GameScreenLevel2::CreateCoin(Vector2D position)
{
	Coin* coin = new Coin(m_renderer, "Images/Coin.png", m_level_map, position);
	m_coin.push_back(coin);
}

void GameScreenLevel2::CreateGoomba(Vector2D position, FACING direction, float speed)
{
	Goomba* goomba = new Goomba(m_renderer, "Images/UGoomba.png", m_level_map, position, direction, speed);
	m_goombas.push_back(goomba);
}

void GameScreenLevel2::LoadMusic(string path)
{
	g_music = Mix_LoadMUS(path.c_str());
	if (g_music == nullptr)
	{
		cout << "Failed to load music. Error: " << Mix_GetError() << endl;
	}
}