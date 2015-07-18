#pragma once
#include <SDL.h>
#include <string>
#include "SDL_image.h"
using namespace std;

class Utils {
public:
	static void FlipHorizontally(SDL_Surface*& image)
	{
		// create a copy of the image
		int w = image->w;
		int h = image->h;
		SDL_Surface* flipped_image = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);

		// lock
		if (SDL_MUSTLOCK(image))
			SDL_LockSurface(image);
		if (SDL_MUSTLOCK(flipped_image))
			SDL_LockSurface(flipped_image);

		Uint32 *pixels = (Uint32 *)image->pixels;
		Uint32 *flippedPixels = (Uint32 *)flipped_image->pixels;

		// loop through pixels
		for (int y = 0; y<h; y++)
			for (int x = 0; x<w; x++)
				// copy pixels, but reverse the x pixels!
				flippedPixels[(y * w) + x] = pixels[(y * w) + (w - x - 1)];

		// unlock
		if (SDL_MUSTLOCK(image))
			SDL_UnlockSurface(image);
		if (SDL_MUSTLOCK(flipped_image))
			SDL_UnlockSurface(flipped_image);

		// free original and assign flipped to it
		SDL_FreeSurface(image);
		image = flipped_image;
	}

	static SDL_Surface* load_image(string filename, bool flipHorizontal = false) {
		SDL_Surface* loadedImage = IMG_Load(filename.c_str());
		if (loadedImage) {
			if (flipHorizontal) {
				FlipHorizontally(loadedImage);
			}

			// create an optimized image
			SDL_Surface* optimizedImage = SDL_DisplayFormatAlpha(loadedImage);

			// free the old image
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