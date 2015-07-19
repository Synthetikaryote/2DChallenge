#pragma once
#include "Sprite.h"

class Character
{
public:
	float x, y;
	float vx, vy;
	float offsetX, offsetY;
	float spawnX, spawnY;
	bool facingLeft;
	bool isAlive;
	Sprite* sprite;
	Character(float spawnX, float spawnY);
	~Character();
	virtual void Update(float elapsed);
	void Draw(SDL_Surface* destination, float offsetX, float offsetY);
	virtual void GotHit(Character* source);
};

