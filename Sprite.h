#pragma once
#include <SDL.h>
#include <map>
#include <string>
using namespace std;

class Sprite
{
	SDL_Surface* spriteSheet;
	map<string, SDL_Rect> frames;
public:
	Sprite(string spriteSheetImage, string spriteSheetData);
	~Sprite();
	void Draw(string frameName, int x, int y, SDL_Surface* destination);
	SDL_Rect GetFrame(string frameName);
};

