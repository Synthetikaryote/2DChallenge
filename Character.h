#pragma once
#include "Sprite.h"

class Character
{
public:
	float x, y;
	float vx, vy;
	float offsetX, offsetY;
	float spawnX, spawnY;
	bool isAlive;
	Sprite* sprite;
	string currentAnimationName;
	int currentAnimationIndex;
	bool isFlipped;
	float framesPerSecond;
	float secondsUntilNextFrame;
	Character(float spawnX, float spawnY, Sprite* sprite);
	~Character();
	virtual void Update(float elapsed);
	void Draw(SDL_Surface* destination, float offsetX, float offsetY);
	virtual void GotHit(Character* source);
	SDL_Rect GetCurrentFrame();
	void SetAnimation(string animationName, bool flipped = false, float framesPerSecond = 20.0f);
};

