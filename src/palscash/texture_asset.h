// Copyright (c) 2018 PalsCash team
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ASSETS_TEXTURE_ASSET_H_
#define ASSETS_TEXTURE_ASSET_H_

#include <SDL2/SDL_rect.h>
#include <string>

struct SDL_Surface;
struct SDL_Texture;

namespace assets {

class texture_asset final {

private:

	SDL_Texture* texture = nullptr;

	SDL_Rect bounds;

public:

	texture_asset(SDL_Renderer* renderer, const std::string filename);

	texture_asset(SDL_Renderer* renderer, SDL_Surface* surface);

	texture_asset(SDL_Renderer* renderer, SDL_Texture* texture);

	virtual ~texture_asset();

	void dispose();

	SDL_Rect& get_bounds();

	SDL_Texture* get_native_texture();

	void update_from_surface(SDL_Surface* surface);

};

}
#endif
