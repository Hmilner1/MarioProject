#pragma once
#include "Tile.h"
#include "texture2D.h"
#include "Constants.h"
#include "Commons.h"
#include "GameScreen.h"
#include "Koopa.h"
#include "Camera.h"
#include "Blocks.h"
#include "LuckyBlock.h"
#include "Coin.h"
#include <vector>

class TileMap
{

public:

	TileMap(SDL_Renderer* renderer);
	~TileMap();

	void GenerateTileMap(char** map, int rows, int columns);
	void DrawTileMap();

	Tile* GetTileAt(int x, int y);

	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }
	bool IsLoaded() { return mLoaded; }

	std::vector<Koopa*> mKoopas;
	std::vector<Coin*> mCoins;


private:
	char** mMap;
	SDL_Renderer* mRenderer;

	std::vector<std::vector<Tile*>>* mTiles;
	int mWidth;
	int mHeight;
	bool mLoaded;
};