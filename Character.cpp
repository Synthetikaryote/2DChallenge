#include "Character.h"


Character::Character() {
}

Character::~Character() {
}

void Character::Update(float elapsed) {

}
void Character::Draw(SDL_Surface* destination, float offsetX, float offsetY) {
	sprite->Draw("p3_walk01", x + offsetX, y + offsetY, destination);
}