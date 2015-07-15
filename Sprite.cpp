#include "Sprite.h"
#include <fstream>
#include "Utils.h"
#include <iostream>
#include <regex>
using namespace std;

Sprite::Sprite(string spriteSheetImage, string spriteSheetData) {
	ifstream spriteData(spriteSheetData);
	string frameName;
	char _;
	SDL_Rect rect;
	string curAnimation;
	while (spriteData >> frameName >> _ >> rect.x >> rect.y >> rect.w >> rect.h) {
		// the animation name is the frame name without the numbers at the end
		string animationName;
		for (int i = frameName.length() - 1; i >= 0; i--) {
			if (!isdigit(frameName[i])) {
				animationName = frameName.substr(0, i + 1);
				break;
			}
		}
		// store the first animation found as the one set for the Sprite
		if (currentAnimationName.empty()) {
			currentAnimationName = animationName;
			currentAnimationIndex = 0;
			framesPerSecond = 20.0f;
		}
		if (curAnimation.empty() || curAnimation.compare(animationName) != 0) {
			curAnimation = animationName;
		}
		frames[curAnimation].push_back(rect);
	}
	spriteData.close();
	spriteSheet = Utils::load_image(spriteSheetImage);
	SDL_SetAlpha(spriteSheet, SDL_SRCALPHA | SDL_RLEACCEL, 255);
}

Sprite::~Sprite()
{
	SDL_FreeSurface(spriteSheet);
}

void Sprite::Update(float elapsed) {
	secondsUntilNextFrame -= elapsed;
	while (secondsUntilNextFrame <= 0) {
		secondsUntilNextFrame += (1.0f / framesPerSecond);
		++currentAnimationIndex;
		if (currentAnimationIndex >= frames[currentAnimationName].size()) {
			currentAnimationIndex = 0;
		}
	}
}

void Sprite::Draw(int x, int y, SDL_Surface* destination) {
	SDL_BlitSurface(spriteSheet, &(frames[currentAnimationName][currentAnimationIndex]), destination, &Utils::MakeRect(x, y, 0, 0));
}

vector<SDL_Rect> Sprite::GetFrames(string animationName) {
	return frames[animationName];
}

void Sprite::SetAnimation(string animationName, float framesPerSecond) {
	this->framesPerSecond = framesPerSecond;
	if (animationName.compare(currentAnimationName) == 0) return;
	currentAnimationName = animationName;
	currentAnimationIndex = 0;
	secondsUntilNextFrame = 1.0f / framesPerSecond;
}