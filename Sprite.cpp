#include "Sprite.h"
#include <fstream>
#include "Utils.h"
#include <iostream>

Sprite::Sprite(string spriteSheetImage, string spriteSheetData) {
	ifstream spriteData(spriteSheetData);
	string frameName;
	char _;
	SDL_Rect rect;
	while (spriteData >> frameName >> _ >> rect.x >> rect.y >> rect.w >> rect.h) {
		frames[frameName] = rect;
	}
	spriteData.close();
	spriteSheet = Utils::load_image(spriteSheetImage);
	SDL_SetAlpha(spriteSheet, SDL_SRCALPHA | SDL_RLEACCEL, 255);
}

Sprite::~Sprite()
{
	SDL_FreeSurface(spriteSheet);
}

void Sprite::Draw(string frameName, int x, int y, SDL_Surface* destination) {
	SDL_BlitSurface(spriteSheet, &(frames[frameName]), destination, &Utils::MakeRect(x, y, 0, 0));
}

SDL_Rect Sprite::GetFrame(string frameName) {
	return frames[frameName];
}