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
	int tileWidth; // tile width
	int tileHeight; // tile height
	float playerStartX;
	float playerStartY;
	SDL_Surface* surface;

	Level(string fileName);
	~Level();

	bool IsBlocked(int col, int row);
};

