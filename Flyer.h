#pragma once
#include "Enemy.h"
class Flyer :
	public Enemy
{
public:
	float speed;
	float sinTime;
	float vy;
	Flyer(float spawnX, float spawnY);
	~Flyer();
	void Update(float elapsed) override;
	void GotHit(Character* source) override;
};

