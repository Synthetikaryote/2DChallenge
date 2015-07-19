#pragma once
#include "Character.h"
class Enemy : public Character
{
public:
	Enemy(float spawnX, float spawnY);
	~Enemy();
	void GotHit(Character* source) override;
};

