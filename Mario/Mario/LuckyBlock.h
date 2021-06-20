#pragma once
#include "Blocks.h"
#include "Collisions.h"
#include <iostream>

class LuckyBlock : public Blocks
{
public:
	LuckyBlock(SDL_Renderer* renderer, std::string imagePath, BlockType type, Vector2D position);
	~LuckyBlock();

	void Update(float deltaTime, SDL_Event e);
	void Render(int camX, int camY);

private:
	bool coinSpawned;
};

