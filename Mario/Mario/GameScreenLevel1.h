#pragma once
#ifndef _GAMESCREENLEVEL1_H
#define _GAMESCREENLEVEL1_H
#include "GameScreen.h"
#include "Collisions.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "Koopa.h"
#include "Goomba.h"
#include "Coin.h"
#include "LevelMap.h"
#include "PowBlock.h"
#include <SDL_TTF.h>
#include <vector>
#include <SDL_mixer.h>

class  Texture2D;
class Character;
class PowBlock;
class GameScreenLevel1 : GameScreen
{
public:
	GameScreenLevel1(SDL_Renderer* renderer);
	~GameScreenLevel1();

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
	void UpdatePowBlock();
	void LoadMusic(string path);


private:
	Mix_Music* g_music = nullptr;
	LevelMap* m_level_map;
	Texture2D* m_background_texture;
	Texture2D* m_background;
	CharacterMario* Mario;
	CharacterLuigi* Luigi;
	PowBlock* m_pow_block;
	vector<Koopa*> m_enemies;
	vector<Goomba*> m_goombas;
	vector<Coin*> m_coin;
	TTF_Font* Retro;
	SDL_Rect textRect;
	SDL_Rect scoreRect;
	Mix_Chunk* coinSound = Mix_LoadWAV("Music/Coin.mp3");
	
	//lvl set up and updates
	bool SetUpLevel();
	void SetLevelMap();
	void DoScreenshake();
	void UpdateEnemies(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction, float speed);
	void CreateCoin(Vector2D position);
	void UpdateCoin(float deltaTime, SDL_Event e);
	void CreateGoomba(Vector2D position, FACING direction, float speed);
	void UpdateGoomba(float deltaTime, SDL_Event e);

	//pow block and respawn
	bool m_screenshake;
	float m_shake_time;
	float m_wobble;
	float m_background_yPos;
	float m_respawn_time = 10.0f;
	float movement_time = 200.0f;
};
#endif