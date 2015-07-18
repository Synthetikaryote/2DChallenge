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
	if (Uber::I().level->IsBlocked(x + dx, y)) {
		if (dx > 0)
			x = floor((x + dx) / Uber::I().level->tileWidth) * Uber::I().level->tileWidth - 0.1f;
		else
			x = ceil((x + dx) / Uber::I().level->tileWidth) * Uber::I().level->tileWidth + 0.1f;
		return true;
	}
	x += dx;
	return false;
}

bool Player::MoveWithCollisionCheckY(float dy) {
	if (Uber::I().level->IsBlocked(x, y + dy)) {
		if (dy > 0)
			y = floor((y + dy) / Uber::I().level->tileHeight) * Uber::I().level->tileHeight - 0.1f;
		else
			y = ceil((x + dy) / Uber::I().level->tileHeight) * Uber::I().level->tileHeight + 0.1f;
		return true;
	}
	y += dy;
	return false;
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

	vy += Uber::I().gravity * elapsed;

	if (MoveWithCollisionCheckY(vy)) {
		onGround = vy > 0.0f;
		vy = 0.0f;
	}
	else {
		onGround = false;
	}
	if (MoveWithCollisionCheckX(vx)) {
		vx = 0.0f;
	}
	
	sprite->SetAnimation(ducking ? "p3_duck" : (!onGround ? "p3_jump" : (vx ? "p3_walk" : "p3_stand")), facingLeft);
	offsetX = -sprite->GetCurrentFrame().w / 2.0f;
	offsetY = -sprite->GetCurrentFrame().h + 4.0f;

	vx = 0.0f;
}
