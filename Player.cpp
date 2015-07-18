#include "Player.h"
#include "Uber.h"
#include <cmath>

Player::Player()
{
	ducking = false;
	facingLeft = false;
	speed = 300.0f;
	sprite = new Sprite("Assets/Player/p3_spritesheet.png", "Assets/Player/p3_spritesheet.txt");
	sprite->SetAnimation("p3_stand");
	x = Uber::I().level->playerStartX + Uber::I().level->tileWidth / 2.0f;
	y = Uber::I().level->playerStartY + Uber::I().level->tileHeight - 1.0f;
}


Player::~Player()
{
}

bool Player::MoveWithCollisionCheckX(float dx) {
	int row = y / Uber::I().level->tileHeight;
	float r = abs(dx);
	if (r < 0.001f)
		return false;
	for (float i = r; i >= 0; i--) {
		float testdx = (i / r) * dx;
		int col = (x + testdx) / Uber::I().level->tileWidth;
		if (!Uber::I().level->IsBlocked(col, row)) {
			x += testdx;
			return i != r;
		}
	}
	return true;
}

bool Player::MoveWithCollisionCheckY(float dy) {
	int col = x / Uber::I().level->tileWidth;
	float r = abs(dy);
	if (r < 0.001f)
		return false;
	for (float i = r; i >= 0; i--) {
		float ratio = i / r;
		float testdy = (i / r) * dy;
		int row = (y + testdy) / Uber::I().level->tileHeight;
		if (!Uber::I().level->IsBlocked(col, row)) {
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
	}
	else {
		ducking = false;
	}
	if (!ducking || !onGround) {
		if (state[SDLK_a]) {
			vx -= speed * elapsed;
		}
		if (state[SDLK_d]) {
			vx += speed * elapsed;
		}
	}
	if ((state[SDLK_w] || state[SDLK_SPACE]) && onGround) {
		vy -= 10.0f;
	}
	
	if (vx != 0)
		facingLeft = vx < 0;
	
	sprite->SetAnimation(ducking ? "p3_duck" : (!onGround ? "p3_jump" : (vx ? "p3_walk" : "p3_stand")), facingLeft);
	offsetX = -sprite->GetCurrentFrame().w / 2.0f;
	offsetY = -sprite->GetCurrentFrame().h + 4.0f;

	vy += Uber::I().gravity * elapsed;

	if (MoveWithCollisionCheckY(vy)) {
		onGround = vy > 0.0f;
		vy = 0.0f;
	}
	else {
		onGround = false;
	}
	MoveWithCollisionCheckX(vx);
	vx = 0.0f;
}
