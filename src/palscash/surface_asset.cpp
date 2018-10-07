// Copyright (c) 2018 PalsCash team
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "surface_asset.h"

#include <SDL2/SDL_endian.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <iostream>

namespace assets {

	surface_asset::surface_asset(const std::string& filename) {

		surface = IMG_Load(filename.c_str());

		if (!surface) {
			std::cerr << "Cannot load surface from file: " << IMG_GetError() << std::endl;
		}

	}

	surface_asset::surface_asset(SDL_Surface* s) {
		surface = s;
	}

	surface_asset::surface_asset(unsigned int width, unsigned int height) {

		surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

		for (auto y = 0; y < height; y++) {
			for (auto x = 0; x < width; x++) {
				put_pixel(x, y, color_to_int(255, 255, 255, 255));
			}
		}

	}

	surface_asset::~surface_asset() {
		if (surface != nullptr) {
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}

	SDL_Surface* surface_asset::get_surface() {
		return surface;
	}

	bool surface_asset::lock() {

		if (SDL_MUSTLOCK(surface)) {
			if (SDL_LockSurface(surface) < 0) {
				std::cerr << "Can't lock surface: " << SDL_GetError() << std::endl;
			}
			return true;
		} else {
			return false;
		}

	}

	void surface_asset::unlock() {
		if (SDL_MUSTLOCK(surface)) {
			SDL_UnlockSurface(surface);
		}
	}

	unsigned long surface_asset::color_to_int(int r, int g, int b, int a) {
		return ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | ((b & 0xFF) << 0);
	}

	void surface_asset::put_pixel(int x, int y, Uint32 pixel) {

		auto bpp = surface->format->BytesPerPixel;

		auto p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

		switch (bpp) {

			case 1:
				*p = pixel;
				break;

			case 2:
				*(Uint16 *) p = pixel;
				break;

			case 3:
				if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
					p[0] = (pixel >> 16) & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = pixel & 0xff;
				} else {
					p[0] = pixel & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = (pixel >> 16) & 0xff;
				}
				break;

			case 4:
				*(Uint32 *) p = pixel;
				break;
		}
	}

	void surface_asset::fill_rect(int x, int y, int width, int height, Uint32 pixel) {

		for (auto xx = x; xx < x + width; xx++) {
			for (auto yy = y; yy < y + height; yy++) {
				put_pixel(xx, yy, pixel);
			}
		}

	}
}

