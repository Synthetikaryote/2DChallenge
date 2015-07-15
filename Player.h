#pragma once
#include "Character.h"
class Player :
	public Character
{
public:
	bool ducking;
	Player();
	~Player();
	void Update(float elapsed) override;
};

