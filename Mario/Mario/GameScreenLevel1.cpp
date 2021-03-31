#include "GameScreenLevel1.h"
#include <sstream>
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
	m_enemies.clear();
	m_coin.clear();
	m_goombas.clear();
}

void GameScreenLevel1::Render()
{
	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->Render();
	}
	//calls the class render function
	for (int i = 0; i < m_coin.size(); i++)
	{
		m_coin[i]->Render();
	}
	for (int i = 0; i < m_goombas.size(); i++)
	{
		m_goombas[i]->Render();
	}
	m_background_texture->Render(Vector2D(0, m_background_yPos), SDL_FLIP_NONE);
	Mario->Render();
	m_pow_block->Render();
	Luigi->Render();

	Retro = TTF_OpenFont("Retro.ttf", 20);
	SDL_Surface* Scoresurf = TTF_RenderText_Solid(Retro,(std::string("Score: ") + to_string(Score)).c_str(), { 0,0,0 });
	SDL_Texture* ScoreTex = SDL_CreateTextureFromSurface(m_renderer, Scoresurf);
	//TTF_CloseFont(Retro);
	scoreRect.x = 0.0f;
	scoreRect.y = 0.0f;
	SDL_RenderCopy(m_renderer, ScoreTex, NULL, &scoreRect);
	SDL_QueryTexture(ScoreTex, NULL, NULL, &scoreRect.w, &scoreRect.h);

	SDL_Surface* Lives = TTF_RenderText_Solid(Retro, (std::string("Lives: ") + to_string(lifeCount)).c_str(), { 0,0,0 });
	SDL_Texture* LivesTex = SDL_CreateTextureFromSurface(m_renderer, Lives);
	textRect.x = 400.0f;
	textRect.y = 0.0f;
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
	Mario->Update(deltaTime, e);
	Luigi->Update(deltaTime, e);
	for (int i = 0; i < m_goombas.size(); i++)
	{
		m_goombas[i]->Update(deltaTime, e);
	}
	UpdatePowBlock(); 
	UpdateEnemies(deltaTime, e);
	UpdateGoomba(deltaTime, e);
	UpdateCoin(deltaTime, e);
	
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
	//loads koopas
	CreateKoopa(Vector2D(150, 32), FACING_RIGHT, KOOPA_SPEED);
	CreateKoopa(Vector2D(325, 32), FACING_LEFT, KOOPA_SPEED);

	CreateGoomba(Vector2D(30, 32), FACING_RIGHT, 20.0f);
	CreateGoomba(Vector2D(-30, 32), FACING_RIGHT, 20.0f);

	CreateCoin(Vector2D(365, 32));
	CreateCoin(Vector2D(395, 32));
	CreateCoin(Vector2D(425, 32));
	//loads PowBlock
	m_pow_block = new PowBlock(m_renderer, m_level_map);

	//screen shake 
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
	if (Collisions::Instance()->Box(Luigi->GetCollisionBox(), m_pow_block->GetCollisionBox()) && m_pow_block->IsAvailable())
	{
		cout << "Box hit" << endl;

		//collided while jumping
		if (Luigi->IsJumping())
		{
			DoScreenshake();
			m_pow_block->TakeHit();
			Luigi->CancelJump();
		}
	}
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
				Mario->foot_position = Mario->foot_position - 0.0001f;
				Luigi->foot_position = Luigi->foot_position - 0.0001f;
				//is the enemy off screen to the left / right?
				if (m_enemies[i]->GetPosition().x < (float)(-m_enemies[i] -> GetCollisionBox().width * 0.5f) ||
					m_enemies[i]->GetPosition().x > SCREEN_WIDTH - (float)(m_enemies[i] -> GetCollisionBox().width * 0.55f))m_enemies[i]->SetAlive(false);
			}
			//now do the update
			m_enemies[i]->Update(deltaTime, e);
			if (m_enemies[i]->GetPosition().x < (float)(-m_enemies[i]->GetCollisionBox().width * 0.5f) ||
				m_enemies[i]->GetPosition().x > SCREEN_WIDTH - (float)(m_enemies[i]->GetCollisionBox().width * 0.55f))
			{
				m_enemies[i]->Turn();
			}
			//check to see if enemy collides with player
			if ((m_enemies[i]->GetPosition().y > 300.0f ||
				m_enemies[i] -> GetPosition().y <= 64.0f) && (m_enemies[i]->GetPosition().x < 64.0f ||
				m_enemies[i]->GetPosition().x >= SCREEN_WIDTH - 96.0f))
			{
				//ignore collisions if behind pipe
			}
			else
			{
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
				if (Collisions::Instance()->Box(m_enemies[i]->GetCollisionBox(), Mario->GetCollisionBox()))
				{
					if (m_enemies[i]->GetInjured())
					{
						Score = Score + 200;
						m_enemies[i]->SetAlive(false);
						cout << Score << endl;
					}
					else
					{
						//kill mario
						Mario->Dead();
					}
				}
				else if(Collisions::Instance()->Box(m_enemies[i]->GetCollisionBox(), Luigi->GetCollisionBox()))
				{
					if (m_enemies[i]->GetInjured())
					{
						Score = Score + 200;
						m_enemies[i]->SetAlive(false);
						cout << Score << endl;
					}
					else
					{
						//kill mario
						Luigi->Dead();
					}
				}
			}
			//if the enemy is no longer alive then schedule it for deletion
			if (!m_enemies[i]->GetAlive())
			{
				cout << "ded" << endl;
				enemyIndexToDelete = i;
			}
		}
		//remove dead enemies -1 each update
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
			//check if the enemy is on the bottom row of tiles
			if (m_goombas[i]->GetPosition().y > 300.0f)
			{
				Mario->foot_position = Mario->foot_position - 0.0001f;
				Luigi->foot_position = Luigi ->foot_position - 0.0001f;

				//is the enemy off screen to the left / right?
				if (m_goombas[i]->GetPosition().x < (float)(-m_goombas[i]->GetCollisionBox().width * 0.5f) ||
					m_goombas[i]->GetPosition().x > SCREEN_WIDTH - (float)(m_goombas[i]->GetCollisionBox().width * 0.55f))m_goombas[i]->SetAlive(false);
			}
			//now do the update
			m_goombas[i]->Update(deltaTime, e);
			if (m_goombas[i]->GetPosition().x < (float)(-m_goombas[i]->GetCollisionBox().width * 0.5f) ||
				m_goombas[i]->GetPosition().x > SCREEN_WIDTH - (float)(m_goombas[i]->GetCollisionBox().width * 0.55f))
			{
				m_goombas[i]->Turn();
			}
			//check to see if enemy collides with player
			if ((m_goombas[i]->GetPosition().y > 300.0f ||
				m_goombas[i]->GetPosition().y <= 64.0f) && (m_goombas[i]->GetPosition().x < 64.0f ||
					m_goombas[i]->GetPosition().x >= SCREEN_WIDTH - 96.0f))
			{
				//ignore collisions if behind pipe
			}
			else
			{
				if (!m_goombas[i]->GetInjured())
				{
					if (Mario->foot_position < m_goombas[i]->foot_position && Collisions::Instance()->Circle(m_goombas[i], Mario))
					{
						Mario->hop();
						m_goombas[i]->TakeDamage();
						if (m_goombas[i]->GetInjured())
						{
							Score = Score + 200;
							m_goombas[i]->SetAlive(false);
							cout << Score << endl;
						}
					}
					else if (Luigi->foot_position < m_goombas[i]->foot_position && Collisions::Instance()->Circle(m_goombas[i], Luigi))
					{
						Luigi->hop();
						m_goombas[i]->TakeDamage();
						if (m_goombas[i]->GetInjured())
						{
							Score = Score + 200;
							m_goombas[i]->SetAlive(false);
							cout << Score << endl;
						}
					}
				}
				if (Collisions::Instance()->Box(m_goombas[i]->GetCollisionBox(), Mario->GetCollisionBox()))
				{

					//kill mario
					Mario->Dead();
				}
				else if (Collisions::Instance()->Box(m_goombas[i]->GetCollisionBox(), Luigi->GetCollisionBox()))
				{


					//kill mario
					Luigi->Dead();
				}
			}
			//if the enemy is no longer alive then schedule it for deletion
			if (!m_goombas[i]->GetAlive())
			{
				cout << "ded" << endl;
				enemyIndexToDelete = i;
			}
		}
		//remove dead enemies -1 each update
		if (enemyIndexToDelete != -1)
		{
			m_goombas.erase(m_goombas.begin() + enemyIndexToDelete);
		}
	}
}

void GameScreenLevel1::UpdateCoin(float deltaTime, SDL_Event e)
{
	if (!m_coin.empty())
	{
		int coinIndexToDelete = -1;
		for (unsigned int i = 0; i < m_coin.size(); i++)
		{
			m_coin[i]->Update(deltaTime, e);
			if (Collisions::Instance()->Circle(m_coin[i], Luigi) || (Collisions::Instance()->Circle(m_coin[i], Mario)))
			{
				Score = Score + 100;
				m_coin[i]->SetAlive(false);
				//Mix_PlayMusic(g_music, -1);
				cout << Score << endl;

				if (!m_coin[i]->GetAlive())
				{
					cout << "Collected" << endl;
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

void GameScreenLevel1::DoScreenshake() 
{
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
		Score = Score + 200;
		cout << Score << endl;
		m_goombas[i]->SetAlive(false);
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

