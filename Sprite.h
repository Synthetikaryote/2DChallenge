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
	float secondsUntilNextFrame;
public:
	string currentAnimationName;
	int currentAnimationIndex;
	bool currentAnimationFlipped;
	float framesPerSecond;
	Sprite(string spriteSheetImage, string spriteSheetData);
	~Sprite();
	void Update(float elapsed);
	void Draw(int x, int y, SDL_Surface* destination);
	SDL_Rect GetCurrentFrame();
	vector<SDL_Rect> GetFrames(string animationName);
	void SetAnimation(string animationName, bool flipped = false, float framesPerSecond = 20.0f);
};

