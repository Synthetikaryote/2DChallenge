#pragma once
#include <SDL.h>
#include <map>
#include <vector>
#include "Utils.h"

class Level
{
public:
	vector<string> level;
	int w; // width
	int h; // height
	int tw; // tile width
	int th; // tile height
	float playerStartX;
	float playerStartY;
	SDL_Surface* surface;

	Level(string fileName);
	~Level();
};

