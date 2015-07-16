#pragma once
#include "Character.h"
class Player :
	public Character
{
public:
	bool ducking;
	float speed;
	float collisionX;
	float collisionY;
	Player();
	~Player();
	void Update(float elapsed) override;
	bool MoveWithCollisionCheck(float dx, float dy);
};

