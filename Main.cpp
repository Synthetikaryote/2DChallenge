#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include "Utils.h"
#include "Sprite.h"
#include "Player.h"
#include "Enemy.h"
using namespace std;

int main(int argc, char* args[]) {
	//The attributes of the screen
	const int SCREEN_WIDTH = 1024;
	const int SCREEN_HEIGHT = 768;
	const int SCREEN_BPP = 32;

	SDL_Surface* screen = NULL;
	map<char, SDL_Surface*> tiles;
	vector<string> level;
	char playerC;
	Player* player;
	vector<Enemy*> enemies;

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

	int w = level[0].length();
	int h = level.size();
	int tw = tiles.begin()->second->w;
	int th = tiles.begin()->second->h;

	// load the player
	player = new Player();
	player->sprite = new Sprite("Assets/Player/p3_spritesheet.png", "Assets/Player/p3_spritesheet.txt");
	player->sprite->SetAnimation("p3_walk");
	player->offsetX = -player->sprite->GetFrames("p3_walk")[0].w / 2.0f;
	player->offsetY = th - player->sprite->GetFrames("p3_walk")[0].h + 4.0f;

	// draw the level and find the player start point
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
				player->x = c * tw + player->offsetX;
				player->y = r * th + player->offsetY;
			}
			else {
				Utils::apply_surface(c * tw, r * th, tiles[level[r][c]], levelSurface);
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

		player->Update(elapsed);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				run = false;
			}
		}
		
		// fill the screen with sky color
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 200, 200));

		// blit the level so that the player is in the center
		float playerOX = -36.0f;
		float playerOY = th - 93.0f;
		float screenX = player->x - SCREEN_WIDTH / 2;
		float screenY = player->y - SCREEN_HEIGHT / 2;
		levelRect.x = screenX;
		levelRect.y = screenY;
		SDL_BlitSurface(levelSurface, &levelRect, screen, NULL);

		// blit the player
		player->Draw(screen, -screenX, -screenY);

		SDL_FillRect(screen, &(Utils::MakeRect(player->x - player->offsetX - screenX, player->y - player->offsetY + th - screenY, 3, 3)), SDL_MapRGB(screen->format, 255, 0, 0));

		// update the screen
		if (SDL_Flip(screen) == -1) {
			return 1;
		}

		int delay = 1000 / 60 - SDL_GetTicks() + lastTicks;
		if (delay>0)
			SDL_Delay(delay);
	}

	// free the memory
	SDL_FreeSurface(levelSurface);
	delete player;
	for (int i = 0; i < enemies.size(); i++) {
		delete enemies[i];
	}
	SDL_Quit(); // frees the screen surface

	return 0;
}