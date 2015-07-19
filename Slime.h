#pragma once
#include "Enemy.h"

class Slime :
	public Enemy
{
public:
	float speed;
	Slime(float spawnX, float spawnY);
	~Slime();
	void Update(float elapsed) override;
	void GotHit(Character* source) override;
};

