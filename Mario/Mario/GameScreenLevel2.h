#pragma once
#ifndef _GAMESCREENLEVEL2_H
#define _GAMESCREENLEVEL2_H
#include "GameScreen.h"
#include "Collisions.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "Koopa.h"
#include "Goomba.h"
#include "Coin.h"
#include "LevelMap.h"
#include "PowBlock.h"
#include "TileMap.h"
#include "Blocks.h"
#include "Camera.h"
#include <SDL_TTF.h>
#include <vector>
#include <SDL_mixer.h>

class  Texture2D;
class Character;
class PowBlock;
class GameScreenLevel1;
class GameScreenLevel2 : GameScreen
{

public:
	GameScreenLevel2(SDL_Renderer* renderer);
	~GameScreenLevel2();
	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
	void LoadMusic(string path);

private:
	Mix_Music* g_music;
	Texture2D* m_background_texture;
	Texture2D* m_background;
	CharacterMario* Mario;
	CharacterLuigi* Luigi;
	LevelMap* m_level_map2;
	vector<Koopa*> m_enemies;
	vector<Goomba*> m_goombas;
	vector<Coin*> m_coin;
	TTF_Font* Retro;
	SDL_Rect textRect;
	SDL_Rect scoreRect;
	TileMap* tileMap;
	//sounds
	Mix_Chunk* coinSound = Mix_LoadWAV("Music/Coin.mp3");
	Mix_Chunk* stompSound = Mix_LoadWAV("Music/Stomp.wav");

	//sets up the lvl 
	bool SetUpLevel();
	void SetLevelMap();
	void UpdateEnemies(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction, float speed);
	void CreateCoin(Vector2D position);
	void UpdateCoin(float deltaTime, SDL_Event e);
	void CreateGoomba(Vector2D position, FACING direction, float speed);
	void UpdateGoomba(float deltaTime, SDL_Event e);
	void SortScore();
	void HandleViewportCollision();
	void UpdateCameraPosition();
	void SetUpTileMap();
	void UpdateLuckyBlock(float deltaTime, SDL_Event e);
	void CheckWin();
	void OnMapCheck(float deltaTime, SDL_Event e);

	float m_background_yPos;
	int highScore;
	float mLastCamXPos;
};
#endif