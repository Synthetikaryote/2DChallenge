#pragma once
#include <SDL.h>
#include <map>
#include <vector>
#include "Utils.h"

class Level
{
public:
	map<char, string> legend;
	vector<string> level;
	int w; // map width, in number of tiles
	int h; // map height, in number of tiles
	int tileWidth; // in pixels
	int tileHeight; // in pixels
	SDL_Surface* surface;

	Level(string fileName);
	~Level();

	bool IsBlockedColRow(int col, int row);
	bool IsBlocked(float x, float y);
	char At(float x, float y);
	string EntityAt(float x, float y);
};

