#pragma once
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

	static void flipHorizontally(SDL_Surface*& image)
	{
		// create a copy of the image
		SDL_Surface* flipped_image = SDL_CreateRGBSurface(SDL_SWSURFACE, image->w, image->h, image->format->BitsPerPixel,
			image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);

		// loop through pixels
		for (int y = 0; y<image->h; y++)
		{
			for (int x = 0; x<image->w; x++)
			{
				// copy pixels, but reverse the x pixels!
				putpixel(flipped_image, x, y, getpixel(image, image->w - x - 1, y));
			}
		}

		// free original and assign flipped to it
		SDL_FreeSurface(image);
		image = flipped_image;
	}
};