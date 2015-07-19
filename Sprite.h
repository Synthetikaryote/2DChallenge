#pragma once
#include <SDL.h>
#include <map>
#include <vector>
#include <string>
using namespace std;

class Sprite
{
	SDL_Surface* spriteSheet;
	SDL_Surface* spriteSheetFlipped;
	map<string, vector<SDL_Rect>> frames;
public:
	Sprite(string spriteSheetImage, string spriteSheetData);
	~Sprite();
	void Sprite::Draw(int x, int y, SDL_Surface* destination, bool flipped, string animationName, int animationIndex);
	vector<SDL_Rect> GetFrames(string animationName);
};

