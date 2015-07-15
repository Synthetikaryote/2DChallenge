#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}

void Player::Update(float elapsed) {
	Character::Update(elapsed);

	const Uint8 *state = SDL_GetKeyState(NULL);
	float speed = 300.0f;
	bool moving = false;
	if (state[SDLK_d]) {
		x += speed * elapsed;
		moving = true;
	}
	if (state[SDLK_a]) {
		x -= speed * elapsed;
		moving = true;
	}
	sprite->SetAnimation(moving ? "p3_walk" : "p3_stand");
}
