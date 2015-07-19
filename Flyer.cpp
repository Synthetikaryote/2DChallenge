#include "Flyer.h"
#include "Uber.h"

Flyer::Flyer(float spawnX, float spawnY) : Enemy(spawnX, spawnY) {
	speed = 100.0f;
	SetAnimation("flyFly", isFlipped, 10.0f);
	x = spawnX + Uber::I().level->tileWidth / 2.0f;
	y = spawnY + Uber::I().level->tileHeight - 1.0f;
}


Flyer::~Flyer() {
}

void Flyer::Update(float elapsed) {
	// this is an override.  do the base class update
	Character::Update(elapsed);

	if (isAlive) {
		// the sprite comes flipped, so technically the flyer is moving backwards
		float dir = isFlipped ? 1.0f : -1.0f;
		x += dir * speed * elapsed;
		sinTime += elapsed;
		y = spawnY + 30.0f * sinf(sinTime * 2.0f);

		// turn around at the flyerBound map marker
		if (Uber::I().level->EntityAt(x, y).compare("flyerBound") == 0) {
			isFlipped = !isFlipped;
			SetAnimation("flyFly", isFlipped, 10.0f);
		}
	}
	else {
		if (y <= Uber::I().level->h * Uber::I().level->tileHeight) {
			vy += Uber::I().gravity * elapsed;
			y += vy;
		}
	}
}

void Flyer::GotHit(Character* source) {
	SetAnimation("flyDead", isFlipped);
	offsetX = -GetCurrentFrame().w / 2.0f;
	offsetY = -GetCurrentFrame().h + 4.0f;
	isAlive = false;
}
