#include "SDL.h"
#include <string>
#include <iostream>
#include "Utils.h"
#include "Player.h"
#include "Enemy.h"
#include "Uber.h"
#include "Level.h"
#include "Slime.h"
using namespace std;

int main(int argc, char* args[]) {
	//The attributes of the screen
	const int SCREEN_WIDTH = 1024;
	const int SCREEN_HEIGHT = 768;
	const int SCREEN_BPP = 32;

	SDL_Surface* screen = NULL;

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
	SDL_WM_SetCaption("Bob the Alien", NULL);

	// load the level
	// this will also spawn the player and enemies
	Uber::I().level = new Level("Assets/map.txt");

	// enemies will share a sprite sheet, so create that first
	Uber::I().enemySprite = new Sprite("Assets/Enemies/enemies_spritesheet.png", "Assets/Enemies/enemies_spritesheet.txt");
	// spawn the player and enemies
	for (int row = 0; row < Uber::I().level->level.size(); row++) {
		for (int col = 0; col < Uber::I().level->level[row].length(); col++) {
			char c = Uber::I().level->level[row][col];
			if (Uber::I().level->legend.count(c)) {
				string entity = Uber::I().level->legend[c];
				if (entity.compare("player") == 0) {
					Uber::I().player = new Player(col * Uber::I().level->tileWidth, row * Uber::I().level->tileHeight);
				}
				else if (entity.compare("slime") == 0) {
					Uber::I().enemies.push_back(new Slime(col * Uber::I().level->tileWidth, row * Uber::I().level->tileHeight));
				}
			}
		}
	}
	
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

		// update the enemies
		for (auto enemy : Uber::I().enemies) {
			enemy->Update(elapsed);
		}
		// update the player
		Uber::I().player->Update(elapsed);
		
		// check for clicking the X on the window
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				run = false;
			}
		}
		
		// fill the screen with sky color
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 150, 200));

		// blit the level so that the player is in the center
		float screenX = Uber::I().player->x - SCREEN_WIDTH / 2;
		float screenY = Uber::I().player->y - SCREEN_HEIGHT / 2;
		levelRect.x = screenX;
		levelRect.y = screenY;
		SDL_BlitSurface(Uber::I().level->surface, &levelRect, screen, NULL);

		// draw the enemies
		for (auto enemy : Uber::I().enemies) {
			enemy->Draw(screen, -screenX, -screenY);
		}
		// draw the player
		Uber::I().player->Draw(screen, -screenX, -screenY);

		// update the screen
		if (SDL_Flip(screen) == -1) {
			return 1;
		}

		// don't exceed 60 fps
		int delay = 1000 / 60 - SDL_GetTicks() + lastTicks;
		if (delay > 0)
			SDL_Delay(delay);
	}

	// free the memory
	delete Uber::I().level;
	delete Uber::I().enemySprite;
	for (auto enemy : Uber::I().enemies) {
		delete enemy;
	}
	delete Uber::I().player;
	SDL_Quit(); // frees the screen surface

	return 0;
}