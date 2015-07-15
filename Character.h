#pragma once
#include "Sprite.h"

class Character
{
public:
	float x, y;
	float vx, vy;
	float offsetX, offsetY;
	Sprite* sprite;
	Character();
	~Character();
	virtual void Update(float elapsed);
	void Draw(SDL_Surface* destination, float offsetX, float offsetY);
};

