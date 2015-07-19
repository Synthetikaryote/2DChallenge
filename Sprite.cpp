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
	spriteSheet = Utils::load_image(spriteSheetImage);
	SDL_SetAlpha(spriteSheet, SDL_SRCALPHA | SDL_RLEACCEL, 255);
	spriteSheetFlipped = Utils::load_image(spriteSheetImage, true);
	SDL_SetAlpha(spriteSheetFlipped, SDL_SRCALPHA | SDL_RLEACCEL, 255);

	while (spriteData >> frameName >> _ >> rect.x >> rect.y >> rect.w >> rect.h) {
		// the animation name is the frame name without the numbers at the end
		string animationName;
		regex expr("(.*[^0-9])");
		smatch match;
		if (regex_search(frameName, match, expr)) {
			animationName = match[0];
			if (curAnimation.empty() || curAnimation.compare(animationName) != 0) {
				curAnimation = animationName;
			}
		}
		frames[curAnimation].push_back(rect);
		frames[curAnimation + "_flipped"].push_back(Utils::MakeRect(spriteSheetFlipped->w - rect.x - rect.w, rect.y, rect.w, rect.h));
	}
	spriteData.close();
}

Sprite::~Sprite()
{
	SDL_FreeSurface(spriteSheet);
	SDL_FreeSurface(spriteSheetFlipped);
}

void Sprite::Draw(int x, int y, SDL_Surface* destination, bool flipped, string animationName, int animationIndex) {
	SDL_BlitSurface(flipped ? spriteSheetFlipped : spriteSheet, &GetFrames(animationName)[animationIndex], destination, &Utils::MakeRect(x, y, 0, 0));
}

vector<SDL_Rect> Sprite::GetFrames(string animationName) {
	return frames[animationName];
}