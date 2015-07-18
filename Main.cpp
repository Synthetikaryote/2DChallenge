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
#include "Level.h"
#include "Uber.h"
using namespace std;

int main(int argc, char* args[]) {
	//The attributes of the screen
	const int SCREEN_WIDTH = 1024;
	const int SCREEN_HEIGHT = 768;
	const int SCREEN_BPP = 32;

	SDL_Surface* screen = NULL;
	Player* player;
	vector<Enemy*> enemies;

	//Initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return 1;
	}
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags) {
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
	Uber::I().level = new Level("Assets/map.txt");
	
	// load the player
	player = new Player();

	SDL_Rect levelRect;
	levelRect.w = SCREEN_WIDTH;
	levelRect.h = SCREEN_HEIGHT;

	bool run = true;
	Uint32 lastTicks = 0;
	while (run) {
		// determine the seconds elapsed since last frame
		Uint32 ticks = SDL_GetTicks();
		float elapsed = (ticks - lastTicks) / 1000.0f;
		lastTicks = ticks;

		// update the player
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
		float screenX = player->x - SCREEN_WIDTH / 2;
		float screenY = player->y - SCREEN_HEIGHT / 2;
		levelRect.x = screenX;
		levelRect.y = screenY;
		SDL_BlitSurface(Uber::I().level->surface, &levelRect, screen, NULL);

		// blit the player
		player->Draw(screen, -screenX, -screenY);

		// update the screen
		if (SDL_Flip(screen) == -1) {
			return 1;
		}

		// keep the framerate at 60 fps
		int delay = 1000 / 60 - SDL_GetTicks() + lastTicks;
		if (delay>0)
			SDL_Delay(delay);
	}

	// free the memory
	delete Uber::I().level;
	delete player;
	for (int i = 0; i < enemies.size(); i++) {
		delete enemies[i];
	}
	SDL_Quit(); // frees the screen surface

	return 0;
}