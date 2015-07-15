#include "Player.h"
#include "Uber.h"

Player::Player()
{
	ducking = false;
}


Player::~Player()
{
}

void Player::Update(float elapsed) {
	Character::Update(elapsed);

	const Uint8 *state = SDL_GetKeyState(NULL);
	float speed = 300.0f;
	if (state[SDLK_d]) {
		vx += speed * elapsed;
	}
	if (state[SDLK_a]) {
		vx -= speed * elapsed;
	}
	if (state[SDLK_s]) {
		ducking = true;
		float doy = Uber::I().level->th - sprite->GetFrames("p3_duck")[0].h + 4.0f - offsetY;
		offsetY += doy;
		y += doy;
	}
	else {
		ducking = false;
		float doy = Uber::I().level->th - sprite->GetFrames("p3_walk")[0].h + 4.0f - offsetY;
		offsetY += doy;
		y += doy;
	}
	
	x += vx;
	sprite->SetAnimation(ducking ? "p3_duck" : (vx ? "p3_walk" : "p3_stand"));

	vx = 0.0f;
}
