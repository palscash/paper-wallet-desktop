// Copyright (c) 2018 PalsCash team
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "sdl2_utils.h"

palscash::sdl2::sdl2() {

	if (SDL_Init( SDL_INIT_EVERYTHING) < 0) {
		std::cerr << "There was an error initializing SDL2: " << SDL_GetError() << std::endl;
		std::terminate();
	}

	// init SDL ttf
	{
		SDL_version compile_version;
		const SDL_version *link_version = TTF_Linked_Version();
		SDL_TTF_VERSION(&compile_version);

		if (TTF_Init() == -1) {
			printf("compiled with SDL_ttf version: %d.%d.%d\n", compile_version.major, compile_version.minor, compile_version.patch);
			printf("running with SDL_ttf version: %d.%d.%d\n", link_version->major, link_version->minor, link_version->patch);
			std::cout << "TTF_Init: " << TTF_GetError() << std::endl;
			exit(2);
		}

	}

	// init SDL image

	{
		auto image_flags = IMG_INIT_PNG;
		auto initted = IMG_Init(image_flags);
		if ((initted & image_flags) != image_flags) {
			{
				SDL_version compile_version;
				const SDL_version *link_version = IMG_Linked_Version();
				SDL_IMAGE_VERSION(&compile_version);
				printf("SDL_Image: Compiled with SDL_image version: %d.%d.%d\n", compile_version.major, compile_version.minor, compile_version.patch);
				printf("SDL_Image: Running with SDL_image version: %d.%d.%d\n", link_version->major, link_version->minor, link_version->patch);
			}
			std::cerr << "IMG_Init: Failed to init required png support!" << std::endl;
			std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
			std::terminate();
		}

	}

}

palscash::sdl2::~sdl2() {

	IMG_Quit();

	TTF_Quit();

	SDL_Quit();

}
