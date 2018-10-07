// Copyright (c) 2018 PalsCash team
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ASSETS_FONT_ASSET_H_
#define ASSETS_FONT_ASSET_H_

#include <SDL2/SDL_ttf.h>
#include <string>

namespace assets {
	class surface_asset;
} /* namespace assets */

namespace assets {

	class font_asset final {

		private:

			TTF_Font* font = nullptr;

			int font_size = 0;

		public:

			font_asset(const std::string filename, int size);

			virtual ~font_asset();

			TTF_Font* get_font();

			surface_asset* generate_text(const std::string text, SDL_Color& color);

	};

}
#endif
