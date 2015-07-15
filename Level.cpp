#include "Level.h"
#include <fstream>

Level::Level(string fileName)
{
	char playerC; // player character on the legend
	map<char, SDL_Surface*> tiles;

	ifstream mapFile(fileName);
	string line;
	while (getline(mapFile, line)) {
		if (line.compare("[legend]") == 0) {
			// read the legend and load the map tiles
			while (getline(mapFile, line) && line.compare("[map]") != 0) {
				if (line.length() > 0) {
					string fileName = line.substr(2);
					if (fileName.compare("empty") == 0) {
					}
					else if (fileName.compare("player") == 0) {
						playerC = line[0];
					}
					else {
						tiles[line[0]] = Utils::load_image("Assets/Tiles/" + fileName);
						SDL_SetAlpha(tiles[line[0]], SDL_SRCALPHA | SDL_RLEACCEL, 255);
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
	tw = tiles.begin()->second->w;
	th = tiles.begin()->second->h;

	// draw the level and find the player start point
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w * tw, h * th, 24, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000);
#else
	surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w * tw, h * th, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000);
#endif
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 100, 200, 200));
	for (int r = 0; r < level.size(); r++) {
		for (int c = 0; c < level[r].length(); c++) {
			if (c == ' ') {
			}
			else if (level[r][c] == playerC) {
				// get the player coordinates from the spawn point marker
				playerStartX = c * tw;
				playerStartY = r * th;
			}
			else {
				Utils::apply_surface(c * tw, r * th, tiles[level[r][c]], surface);
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
