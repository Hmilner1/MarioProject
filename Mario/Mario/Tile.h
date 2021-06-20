#pragma once
#include "texture2D.h"
#include "Commons.h"
#include "Blocks.h"

class Tile
{
public:

	Tile(Blocks* block, CollisionType type);
	~Tile();

	static const int width = 32;
	static const int height = 32;

	const CollisionType& GetCollisionType() const;

	const void Render(int camX, int camY) const;

	Blocks* GetBlock() { return mBlock; }

private:
	Blocks* mBlock;
	CollisionType mCollisionType;
};

