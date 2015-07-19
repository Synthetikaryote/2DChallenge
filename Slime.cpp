#include "Slime.h"
#include "Uber.h"

Slime::Slime(float spawnX, float spawnY) : Enemy(spawnX, spawnY) {
	speed = 70.0f;
	SetAnimation("slimeWalk", isFlipped, 4.0f);
	x = spawnX + Uber::I().level->tileWidth / 2.0f;
	y = spawnY + Uber::I().level->tileHeight - 1.0f;
}

Slime::~Slime()
{
}

void Slime::Update(float elapsed) {
	// this is an override.  do the base class update
	Character::Update(elapsed);

	if (isAlive) {
		// the sprite comes flipped, so technically the slime is moving backwards
		float dir = isFlipped ? 1.0f : -1.0f;
		x += dir * speed * elapsed;

		// turn around if there's a cliff or wall
		if (!Uber::I().level->IsBlocked(x + dir * GetCurrentFrame().w / 2.0f, y + 1) ||
			Uber::I().level->IsBlocked(x + dir * (GetCurrentFrame().w / 2.0f + 1), y)) {
			isFlipped = !isFlipped;
			SetAnimation("slimeWalk", isFlipped, 4.0f);
		}
	}
}

void Slime::GotHit(Character* source) {
	SetAnimation("slimeDead", isFlipped);
	offsetX = -GetCurrentFrame().w / 2.0f;
	offsetY = -GetCurrentFrame().h + 4.0f;
	isAlive = false;
}