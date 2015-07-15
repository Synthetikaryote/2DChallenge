#include <SDL.h>
#include <string>
#include "SDL_image.h"
using namespace std;

class Utils {
public:
	static SDL_Surface* load_image(string filename) {
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

	static void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination) {
		//Make a temporary rectangle to hold the offsets
		SDL_Rect offset;

		//Give the offsets to the rectangle
		offset.x = x;
		offset.y = y;

		//Blit the surface
		SDL_BlitSurface(source, NULL, destination, &offset);
	}

	static SDL_Rect MakeRect(int x, int y, int w, int h) {
		SDL_Rect rect; rect.x = x; rect.y = y; rect.w = w; rect.h = h; return rect;
	}
};