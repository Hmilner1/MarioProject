#pragma once
#ifndef _GAMESCREENLEVEL1_H
#define _GAMESCREENLEVEL1_H
#include "GameScreen.h"
#include "GameScreenManager.h"
#include "Commons.h"
#include "Collisions.h"
#include "Character.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "Coin.h"
#include "LevelMap.h"
#include "PowBlock.h"
#include "Koopa.h"
#include <vector>
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

private:
	Texture2D* m_background_texture;
	Texture2D* m_background;
	CharacterMario* Mario;
	CharacterLuigi* Luigi;
	LevelMap* m_level_map;
	PowBlock* m_pow_block;
	vector<Koopa*> m_enemies;
	vector<Coin*> m_coin;
	
	bool SetUpLevel();
	void SetLevelMap();
	void DoScreenshake();
	void UpdateEnemies(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction, float speed);
	void CreateCoin(Vector2D position);
	void UpdateCoin(float deltaTime, SDL_Event e);

	bool m_screenshake;
	float m_shake_time;
	float m_wobble;
	float m_background_yPos;
	int Score;
};
#endif