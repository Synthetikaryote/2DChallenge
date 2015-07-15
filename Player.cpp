#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}

void Player::Update(float elapsed) {
	const Uint8 *state = SDL_GetKeyState(NULL);
	float speed = 1000.0f;
	if (state[SDLK_d]) {
		x += speed * elapsed;
	}
	if (state[SDLK_a]) {
		x -= speed * elapsed;
	}
	if (state[SDLK_w]) {
		y -= speed * elapsed;
	}
	if (state[SDLK_s]) {
		y += speed * elapsed;
	}
}
