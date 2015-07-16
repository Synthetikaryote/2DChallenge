#include "Player.h"
#include "Uber.h"
#include <cmath>

Player::Player()
{
	ducking = false;
	speed = 300.0f;
	sprite = new Sprite("Assets/Player/p3_spritesheet.png", "Assets/Player/p3_spritesheet.txt");
	sprite->SetAnimation("p3_walk");
	offsetX = -sprite->GetFrames("p3_walk")[0].w / 2.0f;
	offsetY = Uber::I().level->tileHeight - sprite->GetFrames("p3_walk")[0].h + 4.0f;
	x = Uber::I().level->playerStartX + offsetX;
	y = Uber::I().level->playerStartY + offsetY - 10.0f;
}


Player::~Player()
{
}

bool Player::MoveWithCollisionCheck(float dx, float dy) {
	collisionX = x - offsetX + dx;
	collisionY = y - offsetY + Uber::I().level->tileHeight + dy;
	float r = sqrt(dx * dx + dy * dy);
	if (r < 0.001f)
		r = 0.001f;
	for (float i = r; i >= 0; i--) {
		float ratio = i / r;
		float testdx = ratio * dx;
		float testdy = ratio * dy;
		collisionX = x - offsetX + testdx;
		collisionY = y - offsetY + Uber::I().level->tileHeight + testdy;
		int col = collisionX / Uber::I().level->tileWidth;
		int row = collisionY / Uber::I().level->tileHeight;
		if (!Uber::I().level->IsBlocked(col, row)) {
			x += testdx;
			y += testdy;
			return i != r;
		}
	}
	return true;
}

void Player::Update(float elapsed) {
	Character::Update(elapsed);

	const Uint8 *state = SDL_GetKeyState(NULL);

	if (state[SDLK_s]) {
		ducking = true;
		float doy = Uber::I().level->tileHeight - sprite->GetFrames("p3_duck")[0].h + 4.0f - offsetY;
		offsetY += doy;
		y += doy;
	}
	else {
		ducking = false;
		float doy = Uber::I().level->tileHeight - sprite->GetFrames("p3_walk")[0].h + 4.0f - offsetY;
		offsetY += doy;
		y += doy;

		if (state[SDLK_d]) {
			vx += speed * elapsed;
		}
		if (state[SDLK_a]) {
			vx -= speed * elapsed;
		}
		if (state[SDLK_w]) {
			vy -= 10.0f;
		}
	}

	sprite->SetAnimation(ducking ? "p3_duck" : (vx ? "p3_walk" : "p3_stand"));

	vy += Uber::I().gravity * elapsed;

	if (MoveWithCollisionCheck(0, vy)) {
		vy = 0.0f;
	}
	MoveWithCollisionCheck(vx, 0);
	vx = 0.0f;
}
