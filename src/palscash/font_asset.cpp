// Copyright (c) 2018 PalsCash team
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_surface.h>
#include <iostream>
#include <string>

#include "font_asset.h"
#include "surface_asset.h"

namespace assets {

font_asset::font_asset(const std::string name, int size) {

	font = TTF_OpenFont(name.c_str(), size);

	font_size = size;

	if (!font) {
		std::cerr << "TTF_OpenFont: " << TTF_GetError() << std::endl;
	}

}

TTF_Font* font_asset::get_font() {
	return font;
}

font_asset::~font_asset() {
	if (font != nullptr) {
		TTF_CloseFont(font);
		font = nullptr;
	}
}

surface_asset* font_asset::generate_text(const std::string text, SDL_Color& sdl_color) {

	auto sdl_surface = TTF_RenderText_Blended(font, text.c_str(), sdl_color);

	auto tex = new surface_asset(sdl_surface);

	return tex;

}

}

