#include "TileMap.h"

TileMap::TileMap(SDL_Renderer* renderer)
{
	mRenderer = renderer;

	mLoaded = false;

	mMap = nullptr;
}

TileMap::~TileMap()
{
	delete mRenderer;
	mRenderer = nullptr;

	for (Koopa* koopa : mKoopas)
	{
		delete koopa;
		koopa = nullptr;
	}

}

void TileMap::GenerateTileMap(char** map, int rows, int columns)
{
	mMap = new char* [rows];
	for (unsigned char i = 0; i < rows; i++)
	{
		mMap[i] = new char[columns];
	}

	mWidth = columns;
	mHeight = rows;



	mTiles = new std::vector<std::vector<Tile*>>(columns, std::vector<Tile*>(rows));

	mKoopas = std::vector<Koopa*>();

	for (unsigned int row = 0; row < rows; row++)
	{
		for (unsigned int column = 0; column < columns; column++)
		{
			mMap[row][column] = map[row][column];

			char type = mMap[row][column];

			switch (type)
			{
			case '1':
				(*mTiles)[column][row] = new Tile(new Blocks(mRenderer, "Images/block.png", Blocks::BlockType::BLOCK_PLAT, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '2':
				(*mTiles)[column][row] = new Tile(new Blocks(mRenderer, "Images/floor.png", Blocks::BlockType::BLOCK_FLOOR, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '3':
				(*mTiles)[column][row] = new Tile(new LuckyBlock(mRenderer, "Images/QuestionMarkBlock.png", Blocks::BlockType::BLOCK_LUCKY, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '4':
				(*mTiles)[column][row] = new Tile(new Blocks(mRenderer, "Images/Step.png", Blocks::BlockType::BLOCK_STEP, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '5':
				(*mTiles)[column][row] = new Tile(new Blocks(mRenderer, "Images/PipeTopLeft.png", Blocks::BlockType::BLOCK_PIPE, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '6':
				(*mTiles)[column][row] = new Tile(new Blocks(mRenderer, "Images/PipeTopRight.png", Blocks::BlockType::BLOCK_PIPE, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '7':
				(*mTiles)[column][row] = new Tile(new Blocks(mRenderer, "Images/PipeBottomLeft.png", Blocks::BlockType::BLOCK_PIPE, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case '8':
				(*mTiles)[column][row] = new Tile(new Blocks(mRenderer, "Images/PipeBottomRight.png", Blocks::BlockType::BLOCK_PIPE, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case 'p':
				(*mTiles)[column][row] = new Tile(new Blocks(mRenderer, "Images/block-blue.png", Blocks::BlockType::BLOCK_PLAT, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case 'f':
				(*mTiles)[column][row] = new Tile(new Blocks(mRenderer, "Images/floor-blue.png", Blocks::BlockType::BLOCK_FLOOR, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case 's':
				(*mTiles)[column][row] = new Tile(new Blocks(mRenderer, "Images/step-blue.png", Blocks::BlockType::BLOCK_STEP, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case 'q':
				(*mTiles)[column][row] = new Tile(new LuckyBlock(mRenderer, "Images/QuestionMarkBlock-blue.png", Blocks::BlockType::BLOCK_LUCKY, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			case 'w':
				(*mTiles)[column][row] = new Tile(new Blocks(mRenderer, "Images/Flag.png", Blocks::BlockType::BLOCK_STEP, Vector2D(column * 32, row * 32)), CollisionType::TILE_SOLID);
				break;
			}

		}
	}

	mLoaded = true;
}

void TileMap::DrawTileMap()
{
	for (int x = 0; x < GetWidth(); x++)
	{
		for (int y = 0; y < GetHeight(); y++)
		{
			Tile* t = GetTileAt(x, y);
			if (t != nullptr)
				t->Render(Camera::GetInstance()->GetPosition().x, Camera::GetInstance()->GetPosition().y);
		}
	}
}

Tile* TileMap::GetTileAt(int x, int y)
{
	if (x >= GetWidth() || x < 0) return nullptr;
	if (y >= GetHeight() || y < 0) return nullptr;

	Tile* t = mTiles->at(x).at(y);

	if (t != nullptr)
		return t;

	return nullptr;
}


