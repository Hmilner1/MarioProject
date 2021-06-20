#include "Blocks.h"
#include <iostream>

Blocks::Blocks(SDL_Renderer* renderer, std::string imagePath, BlockType type, Vector2D pos)
{
	m_renderer = renderer;

	mSprite = new Texture2D(m_renderer);

	if (!mSprite->LoadFromFile(imagePath))
	{
		std::cerr << "Could not load file " << imagePath << std::endl;
	}

	mSingleSpriteWidth = mSprite->GetWidth();
	mSingleSpriteHeight = mSprite->GetHeight();

	mBlockType = type;
	mAvailable = true;

	mPosition = pos;
}

Blocks::~Blocks()
{

}

void Blocks::Update(float deltaTime, SDL_Event e)
{

}

void Blocks::Render(int camX, int camY)
{
	mSprite->Render(mPosition.x - camX, mPosition.y - camY, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
}

void Blocks::SetAvailable(bool available)
{
	mAvailable = available;
}

float Blocks::GetWidth()
{
	return mSingleSpriteWidth;
}

float Blocks::GetHeight()
{
	return mSingleSpriteHeight;
}

void Blocks::SetPosition(Vector2D pos)
{
	mPosition = pos;
}
