#pragma once
#include <SDL.h>
#include <map>
#include <vector>
#include "Utils.h"

class Level
{
public:
	vector<string> level;
	int w; // map width, in number of tiles
	int h; // map height, in number of tiles
	int tileWidth; // in pixels
	int tileHeight; // in pixels
	float playerStartX; // top-left corner of the tile where the player starts
	float playerStartY;
	SDL_Surface* surface;

	Level(string fileName);
	~Level();

	bool IsBlockedColRow(int col, int row);
	bool IsBlocked(float x, float y);
	char Level::At(float x, float y);
};

