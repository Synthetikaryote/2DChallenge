#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

SDL_Surface* loadSurface(string path);

//The attributes of the screen
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int SCREEN_BPP = 32;

SDL_Surface* screen = NULL;
map<char, SDL_Surface*> tiles;
vector<string> level;
char playerC;
float playerX;
float playerY;

//The event structure that will be used
SDL_Event event;

SDL_Surface* load_image(string filename) {
	SDL_Surface* loadedImage = IMG_Load(filename.c_str());
	if (loadedImage) {
		//Create an optimized image
		SDL_Surface* optimizedImage = SDL_DisplayFormatAlpha(loadedImage);

		//Free the old image
		SDL_FreeSurface(loadedImage);

		return optimizedImage;
	}
	else {
		string error = IMG_GetError();
	}
	return NULL;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
	//Make a temporary rectangle to hold the offsets
	SDL_Rect offset;

	//Give the offsets to the rectangle
	offset.x = x;
	offset.y = y;

	//Blit the surface
	SDL_BlitSurface(source, NULL, destination, &offset);
}

int main(int argc, char* args[]) {
	//Initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return 1;
	}
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if (initted & flags != flags) {
		cout << "could not init SDL_Image" << endl;
		cout << "Reason: " << IMG_GetError() << endl;
	}

	//Set up the screen
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE);
	
	//If there was an error in setting up the screen
	if (screen == NULL) {
		return 1;
	}

	//Set the window caption
	SDL_WM_SetCaption("2DChallenge", NULL);

	// load the level
	ifstream mapFile("Assets/map.txt");
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
						tiles[line[0]] = load_image("Assets/Tiles/" + fileName);
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

	// draw the level
	int w = level[0].length();
	int h = level.size();
	int tw = tiles.begin()->second->w;
	int th = tiles.begin()->second->h;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	SDL_Surface* levelSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, w * tw, h * th, 24, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000);
#else
	SDL_Surface* levelSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, w * tw, h * th, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000);
#endif
	SDL_FillRect(levelSurface, NULL, SDL_MapRGB(levelSurface->format, 100, 200, 200));
	for (int r = 0; r < level.size(); r++) {
		for (int c = 0; c < level[r].length(); c++) {
			if (c == ' ') {
			}
			else if (level[r][c] == playerC) {
				// get the player coordinates from the spawn point marker
				playerX = c * tw;
				playerY = r * th;
			}
			else {
				apply_surface(c * tw, r * th, tiles[level[r][c]], levelSurface);
			}
		}
	}
	// we don't need the tiles anymore, so free them all
	for (auto i = tiles.begin(); i != tiles.end(); i++) {
		SDL_FreeSurface(i->second);
	}
	// optimize the level
	SDL_Surface* temp = SDL_DisplayFormat(levelSurface);
	SDL_FreeSurface(levelSurface);
	levelSurface = temp;

	SDL_Rect levelRect;
	levelRect.w = SCREEN_WIDTH;
	levelRect.h = SCREEN_HEIGHT;

	bool run = true;
	Uint32 lastTicks = 0;
	while (run) {
		Uint32 ticks = SDL_GetTicks();
		float elapsed = (ticks - lastTicks) / 1000.0f;
		lastTicks = ticks;

		const Uint8 *state = SDL_GetKeyState(NULL);
		float speed = 1000.0f;
		if (state[SDLK_f]) {
			playerX += speed * elapsed;
		}
		if (state[SDLK_s]) {
			playerX -= speed * elapsed;
		}
		if (state[SDLK_e]) {
			playerY -= speed * elapsed;
		}
		if (state[SDLK_d]) {
			playerY += speed * elapsed;
		}

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				run = false;
			}
		}
		
		// fill the screen with sky color
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 200, 200));

		// blit the level so that the player is in the center
		levelRect.x = playerX - SCREEN_WIDTH / 2;
		levelRect.y = playerY - SCREEN_HEIGHT / 2;
		SDL_BlitSurface(levelSurface, &levelRect, screen, NULL);

		//Update the screen
		if (SDL_Flip(screen) == -1) {
			return 1;
		}

		int delay = 1000 / 60 - SDL_GetTicks() + lastTicks;
		if (delay>0)
			SDL_Delay(delay);
	}

	//Quit SDL
	SDL_Quit();

	//Return
	return 0;
}