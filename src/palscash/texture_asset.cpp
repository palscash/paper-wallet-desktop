// Copyright (c) 2018 PalsCash team
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <iostream>

#include "texture_asset.h"
#include "surface_asset.h"

namespace assets {

texture_asset::texture_asset(SDL_Renderer* renderer, const std::string filename) {

	assets::surface_asset s(filename);

	this->texture = SDL_CreateTextureFromSurface(renderer, s.get_surface());

	if (!texture) {
		std::cerr << "Cannot create texture from file: " << SDL_GetError() << std::endl;
		return;
	}

	SDL_QueryTexture(texture, nullptr, nullptr, &bounds.w, &bounds.h);

	std::cout << "Loaded texture: " << filename << " " << std::to_string(bounds.w) << "x" << std::to_string(bounds.h) << std::endl;

}

texture_asset::texture_asset(SDL_Renderer* renderer, SDL_Surface* surface) {

	this->texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (!texture) {
		std::cerr << "Cannot create texture from surface: " << SDL_GetError() << std::endl;
		return;
	}

	SDL_QueryTexture(texture, nullptr, nullptr, &bounds.w, &bounds.h);

}

texture_asset::~texture_asset() {
	dispose();
}

void texture_asset::dispose() {
	if (this->texture != nullptr) {

		SDL_DestroyTexture(texture);
		texture = nullptr;

		std::cout << "Destroyed texture" << std::endl;

	}
}

texture_asset::texture_asset(SDL_Renderer* renderer, SDL_Texture* texture) {
	this->texture = texture;
	SDL_QueryTexture(texture, nullptr, nullptr, &bounds.w, &bounds.h);
	std::cout << "Create texture asset: " << std::to_string(bounds.w) << "x" << std::to_string(bounds.h) << std::endl;

}

SDL_Rect& texture_asset::get_bounds() {
	return bounds;
}

SDL_Texture* texture_asset::get_native_texture() {
	return texture;
}

void texture_asset::update_from_surface(SDL_Surface* surface) {
	SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
}

}
