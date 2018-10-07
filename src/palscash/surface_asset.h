// Copyright (c) 2018 PalsCash team
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ASSETS_SURFACE_ASSET_H_
#define ASSETS_SURFACE_ASSET_H_

#include <SDL2/SDL_stdinc.h>
#include <string>

struct SDL_Surface;

namespace assets {

	class surface_asset final {

		private:

			SDL_Surface* surface = nullptr;

		public:

			surface_asset(const std::string& filename);

			surface_asset(unsigned int width, unsigned int height);

			surface_asset(SDL_Surface* surface);

			unsigned long color_to_int(int r, int g, int b, int a);

			~surface_asset();

			SDL_Surface* get_surface();

			void unlock();

			bool lock();

			void fill_rect(int x, int y, int width, int height, Uint32 pixel);

			void put_pixel(int x, int y, Uint32 pixel);

	};

}
#endif
