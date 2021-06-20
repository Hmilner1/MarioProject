#pragma once
#include <SDL.h>
#include "texture2D.h"
#include "Commons.h"

class Blocks
{
public:

	enum class BlockType
	{
		BLOCK_PLAT,
		BLOCK_FLOOR,
		BLOCK_LUCKY,
		BLOCK_STEP,
		BLOCK_PIPE,
	};

	Blocks(SDL_Renderer* renderer, std::string imagePath, BlockType type, Vector2D pos);
	~Blocks();

	virtual void Update(float deltaTime, SDL_Event e);
	virtual void Render(int camX, int camY);

	Rect2D GetCollisionBox() { return Rect2D(mPosition.x, mPosition.y, mSingleSpriteWidth, mSingleSpriteHeight); }

	BlockType GetBlockType() { return mBlockType; }

	bool IsAvailable() { return mAvailable; }
	void SetAvailable(bool available);

	float GetWidth();
	float GetHeight();

	Vector2D GetPosition() { return mPosition; }
	void SetPosition(Vector2D pos);

protected:
	SDL_Renderer* m_renderer;
	Texture2D* mSprite;
	Vector2D mPosition;

	BlockType mBlockType;

	bool mAvailable;

	float mSingleSpriteWidth;
	float mSingleSpriteHeight;
};

