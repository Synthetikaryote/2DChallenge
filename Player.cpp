#include "Player.h"
#include "Uber.h"
#include <cmath>

Player::Player(float spawnX, float spawnY) : Character(spawnX, spawnY)
{
	ducking = false;
	facingLeft = false;
	speed = 300.0f;
	sprite = new Sprite("Assets/Player/p3_spritesheet.png", "Assets/Player/p3_spritesheet.txt");
	Respawn();
}

Player::~Player()
{
}

void Player::Respawn() {
	ducking = false;
	facingLeft = false;
	speed = 300.0f;
	sprite->SetAnimation("p3_stand");
	x = spawnX + Uber::I().level->tileWidth / 2.0f;
	y = spawnY + Uber::I().level->tileHeight - 1.0f;
}

bool Player::MoveWithCollisionCheckX(float dx) {
	float d = signbit(dx) ? -1.0f : 1.0f;
	// check one tile at a time in case dx is more than a tile, so tiles can't be skipped
	// start the loop at dx modulo tileWidth and add tileWidth until it passes dx
	for (float dist = fmodf(abs(dx), Uber::I().level->tileWidth); dist <= abs(dx); dist += Uber::I().level->tileWidth) {
		float c = d * dist;
		if (Uber::I().level->IsBlocked(x + c, y)) {
			if (dx > 0)
				x = floor((x + c) / Uber::I().level->tileWidth) * Uber::I().level->tileWidth - 0.1f;
			else if (dx < 0)
				x = ceil((x + c) / Uber::I().level->tileWidth) * Uber::I().level->tileWidth + 0.1f;
			return true;
		}
	}
	x += dx;
	return false;
}

bool Player::MoveWithCollisionCheckY(float dy) {
	float d = signbit(dy) ? -1.0f : 1.0f;
	// check one tile at a time in case dy is more than a tile, so tiles can't be skipped
	// start the loop at dy modulo tileHeight and add tileHeight until it passes dy
	for (float dist = fmodf(abs(dy), Uber::I().level->tileHeight); dist <= abs(dy); dist += Uber::I().level->tileHeight) {
		float c = d * dist;
		if (Uber::I().level->IsBlocked(x, y + c)) {
			if (dy > 0)
				y = floor((y + c) / Uber::I().level->tileHeight) * Uber::I().level->tileHeight - 0.1f;
			else
				y = ceil((y + c) / Uber::I().level->tileHeight) * Uber::I().level->tileHeight + 0.1f;
			return true;
		}
	}
	y += dy;
	return false;
}


void Player::Update(float elapsed) {
	// this is an override.  do the base class update
	Character::Update(elapsed);

	// get ducking, jumping and movement key input
	const Uint8 *state = SDL_GetKeyState(NULL);
	ducking = state[SDLK_s];
	if (state[SDLK_a]) {
		vx -= speed * elapsed;
	}
	if (state[SDLK_d]) {
		vx += speed * elapsed;
	}
	if ((state[SDLK_w] || state[SDLK_SPACE]) && onGround) {
		vy -= 7.0f;
	}
	
	// determine the facing direction
	if (vx != 0.0f)
		facingLeft = vx < 0;

	// can't move while ducking unless in the air
	if (ducking && onGround) {
		vx = 0.0f;
	}

	// apply gravity
	vy += Uber::I().gravity * elapsed;

	// do movement with collision checks.  y, then x
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

	// fall death
	if (Uber::I().level->EntityAt(x, y).compare("death") == 0) {
		Respawn();
	}
	
	// determine the animation based on the player state
	sprite->SetAnimation(ducking ? "p3_duck" : (!onGround ? "p3_jump" : (vx ? "p3_walk" : "p3_stand")), facingLeft);

	// recalculate the visual offset depending on the sprite frame
	offsetX = -sprite->GetCurrentFrame().w / 2.0f;
	offsetY = -sprite->GetCurrentFrame().h + 4.0f;

	// always zero out the x velocity because that's how platformers work!
	vx = 0.0f;
}
