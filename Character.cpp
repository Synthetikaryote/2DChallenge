#include "Character.h"


Character::Character(float spawnX, float spawnY, Sprite* sprite) {
	this->spawnX = spawnX;
	this->spawnY = spawnY;
	this->sprite = sprite;
	isAlive = true;
}

Character::~Character() {
}

void Character::Update(float elapsed) {
	secondsUntilNextFrame -= elapsed;
	while (secondsUntilNextFrame <= 0) {
		secondsUntilNextFrame += (1.0f / framesPerSecond);
		++currentAnimationIndex;
		if (currentAnimationIndex >= sprite->GetFrames(currentAnimationName).size()) {
			currentAnimationIndex = 0;
		}
	}
}
void Character::Draw(SDL_Surface* destination, float offsetX, float offsetY) {
	string animationName = currentAnimationName;
	if (isFlipped)
		animationName += "_flipped";
	sprite->Draw(x + this->offsetX + offsetX, y + this->offsetY + offsetY, destination, isFlipped, animationName, currentAnimationIndex);
}

SDL_Rect Character::GetCurrentFrame() {
	string animationName = currentAnimationName;
	if (isFlipped)
		animationName += "_flipped";
	return sprite->GetFrames(animationName)[currentAnimationIndex];
}

void Character::SetAnimation(string animationName, bool flipped, float framesPerSecond) {
	this->framesPerSecond = framesPerSecond;
	if (animationName.compare(currentAnimationName) == 0) {
		if (flipped == isFlipped) {
			return;
		}
	}
	else {
		currentAnimationName = animationName;
		currentAnimationIndex = 0;
		secondsUntilNextFrame = 1.0f / framesPerSecond;
	}
	isFlipped = flipped;
}

void Character::GotHit(Character* source) {

}