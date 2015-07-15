#pragma once
#include "Character.h"
class Player :
	public Character
{
public:
	Player();
	~Player();
	void Update(float elapsed) override;
};

