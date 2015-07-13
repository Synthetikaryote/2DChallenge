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
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
//The surfaces that will be used
SDL_Surface* message = NULL;
SDL_Surface* background = NULL;
SDL_Surface* screen = NULL;
map<char, SDL_Surface*> tiles;
vector<string> level;
char playerC;
float playerX;
float playerY;

//The event structure that will be used
SDL_Event event;

SDL_Surface* load_image(string filename)
{
	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//The optimized image that will be used
	SDL_Surface* optimizedImage = NULL;

	//Load the image using SDL_image
	loadedImage = IMG_Load(filename.c_str());

	//If the image loaded
	if (loadedImage != NULL)
	{
		//Create an optimized image
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);

		//Free the old image
		SDL_FreeSurface(loadedImage);
	}
	else {
		string error = IMG_GetError();
	}

	//Return the optimized image
	return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
	//Make a temporary rectangle to hold the offsets
	SDL_Rect offset;

	//Give the offsets to the rectangle
	offset.x = x;
	offset.y = y;

	//Blit the surface
	SDL_BlitSurface(source, NULL, destination, &offset);
}

int main(int argc, char* args[])
{

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
	SDL_WM_SetCaption("3DChallenge", NULL);

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

	// fill the screen with sky
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 100, 200, 200));
	
	// draw the map
	int w = level[0].length();
	int h = level.size();
	int tw = tiles.begin()->second->w;
	int th = tiles.begin()->second->h;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	SDL_Surface* levelSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, w * tw, h * th, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
#else
	SDL_Surface* levelSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, w * tw, h * th, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
#endif
	SDL_FillRect(levelSurface, NULL, SDL_MapRGBA(levelSurface->format, 0, 0, 0, 0));
	for (int r = 0; r < level.size(); r++) {
		for (int c = 0; c < level[r].length(); c++) {
			if (c == ' ') {
				
			}
			else if (level[r][c] == playerC) {
				playerX = c * tw;
				playerY = r * th;
			}
			else {
				apply_surface(c * tw, r * th, tiles[level[r][c]], levelSurface);
			}
		}
	}

	// blit the level so that the player is in the center
	SDL_Rect sourceRect;
	sourceRect.x = playerX - SCREEN_WIDTH / 2;
	sourceRect.y = playerY - SCREEN_HEIGHT / 2;
	sourceRect.w = SCREEN_WIDTH;
	sourceRect.h = SCREEN_HEIGHT;
	SDL_BlitSurface(levelSurface, &sourceRect, screen, NULL);

	//Update the screen
	if (SDL_Flip(screen) == -1)
	{
		return 1;
	}

	//Wait 2 seconds
	SDL_Delay(2000);
	return 0;

	//Free the surfaces
	SDL_FreeSurface(message);
	SDL_FreeSurface(background);

	//Quit SDL
	SDL_Quit();

	//Return
	return 0;
}