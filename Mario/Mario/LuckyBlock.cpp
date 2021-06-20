#include "LuckyBlock.h"

LuckyBlock::LuckyBlock(SDL_Renderer* renderer, std::string imagePath, BlockType type, Vector2D position) : Blocks(renderer, imagePath, type, position)
{
	mSingleSpriteWidth = mSprite->GetWidth() / 2;
	mAvailable = true;
	coinSpawned = false;
}

LuckyBlock::~LuckyBlock()
{
	delete m_renderer;
	m_renderer = nullptr;

	delete mSprite;
	mSprite = nullptr;
}

void LuckyBlock::Update(float deltaTime, SDL_Event e)
{
	if (!IsAvailable())
	{
		if (!coinSpawned)
		{
			coinSpawned = true;
		}
	}
}

void LuckyBlock::Render(int camX, int camY)
{
	int left = 0;

	if (IsAvailable())
		left = 0;
	else left = mSingleSpriteWidth;

	SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };

	mSprite->Render(mPosition.x - camX, mPosition.y - camY, &portionOfSpritesheet, 0.0, nullptr, SDL_FLIP_NONE);
}
