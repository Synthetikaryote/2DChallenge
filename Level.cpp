#include "Level.h"
#include <fstream>
#include <sys/stat.h>
#include "Uber.h"
#include "Character.h"
#include "Slime.h"

// http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
inline bool exists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

Level::Level(string fileName)
{
	map<char, SDL_Surface*> tiles;

	ifstream mapFile(fileName);
	string line;
	while (getline(mapFile, line)) {
		if (line.compare("[legend]") == 0) {
			// read the legend and load the map tiles
			while (getline(mapFile, line) && line.compare("[map]") != 0) {
				if (line.length() > 0) {
					char c = line[0];
					string fileName = line.substr(2);
					if (exists("Assets/Tiles/" + fileName)) {
						tiles[line[0]] = Utils::load_image("Assets/Tiles/" + fileName);
						SDL_SetAlpha(tiles[line[0]], SDL_SRCALPHA | SDL_RLEACCEL, 255);
					}
					else {
						legend[c] = fileName;
					}
				}
			}
			// read the map
			while (getline(mapFile, line)) {
				level.push_back(line);
			}
		}
	}

	w = level[0].length();
	h = level.size();
	tileWidth = tiles.begin()->second->w;
	tileHeight = tiles.begin()->second->h;

	// draw the level and find the player start point
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w * tileWidth, h * tileHeight, 24, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000);
#else
	surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w * tileWidth, h * tileHeight, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000);
#endif
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 100, 150, 200));
	for (int r = 0; r < (int)level.size(); r++) {
		for (int c = 0; c < (int)level[r].length(); c++) {
			if (tiles.count(level[r][c])) {
				Utils::apply_surface(c * tileWidth, r * tileHeight, tiles[level[r][c]], surface);
			}
		}
	}
	// we don't need the tiles anymore, so free them all
	for (auto i = tiles.begin(); i != tiles.end(); i++) {
		SDL_FreeSurface(i->second);
	}
	// optimize the level
	SDL_Surface* temp = SDL_DisplayFormat(surface);
	SDL_FreeSurface(surface);
	surface = temp;
}


Level::~Level()
{
	SDL_FreeSurface(surface);
}

bool Level::IsBlockedColRow(int col, int row) {
	if (col < 0 || col >= w || row < 0 || row >= h)
		return true;
	char c = level[row][col];
	return string(" PXS").find(c) == string::npos;
}
bool Level::IsBlocked(float x, float y) {
	return IsBlockedColRow(x / tileWidth, y / tileHeight);
}

char Level::At(float x, float y) {
	int col = x / tileWidth;
	int row = y / tileHeight;
	if (col < 0 || col >= w || row < 0 || row >= h)
		return '\0';
	return level[row][col];
}

string Level::EntityAt(float x, float y) {
	char c = At(x, y);
	return legend.count(c) ? legend[c] : '\0';
}