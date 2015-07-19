#include "Character.h"


Character::Character(float spawnX, float spawnY) {
	this->spawnX = spawnX;
	this->spawnY = spawnY;
}

Character::~Character() {
}

void Character::Update(float elapsed) {
	sprite->Update(elapsed);
}
void Character::Draw(SDL_Surface* destination, float offsetX, float offsetY) {
	sprite->Draw(x + this->offsetX + offsetX, y + this->offsetY + offsetY, destination);
}