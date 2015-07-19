#pragma once
#include "Character.h"
class Player :
	public Character
{
public:
	bool onGround;
	bool ducking;
	float speed;
	Player(float spawnX, float spawnY);
	~Player();
	void Respawn();
	void Update(float elapsed) override;
	bool MoveWithCollisionCheckX(float dx);
	bool MoveWithCollisionCheckY(float dy);
	bool MoveWithCollisionCheck(float dx, float dy);
};

