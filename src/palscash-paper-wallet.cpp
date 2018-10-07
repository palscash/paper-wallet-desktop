// Copyright (c) 2018 PalsCash team
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <iostream>
#include <string>

#include "args.hpp"
#include "palscash/font_asset.h"
#include "palscash/key_pair.h"
#include "palscash/open_ssl_helper.h"
#include "palscash/surface_asset.h"
#include "qr/QrCode.hpp"
#include "sdl2_utils.h"

const std::string VERSION = "v1.0.1";

// Prints the given QR Code to the console.
void draw_qr_code(const qrcodegen::QrCode &qr, assets::surface_asset& surf) {

	auto yy = 0;
	auto dot_size = 12;

	for (auto y = 0; y < qr.getSize(); y++) {

		auto xx = 0;

		for (auto x = 0; x < qr.getSize(); x++) {

			if (qr.getModule(x, y)) {
				surf.fill_rect(xx, yy, dot_size, dot_size, surf.color_to_int(0, 0, 0, 255));
			}

			xx += dot_size;

		}

		yy += dot_size;

	}

}

int main(int argc, char** argv) {

	palscash::open_ssl_helper ossl;

	args::Group arguments("Arguments");
	args::HelpFlag help_arg(arguments, "help", "Display this help and exit", { 'h', "help" });
	args::ValueFlag<std::string> priv_key_arg(arguments, "privateKey", "Private key (don't specify it to generate a new account)", { "privatekey" });
	args::Flag version_arg(arguments, "version", "Output version and exit", { "version", "v" });

	args::ArgumentParser parser("PalsCash Paper Wallet Generator (PalsCash.org) " + VERSION);

	args::GlobalOptions globals(parser, arguments);

	try {
		parser.ParseCLI(argc, argv);
	} catch (args::Help) {
		std::cout << parser;
		return EXIT_SUCCESS;
	} catch (args::Error& e) {
		std::cerr << e.what() << std::endl << parser;
		return EXIT_FAILURE;
	}

	// Output version and exit
	if (version_arg) {
		std::cout << VERSION << std::endl;
		return EXIT_SUCCESS;
	}

	// init sdl2
	palscash::sdl2 sdl2;

	// get private key from command line
	// or generate a new one
	auto private_key = priv_key_arg.Get();
	std::string public_address;

	if (private_key.empty()) {
		palscash::key_pair kp;
		private_key = kp.get_private_key();
		public_address = kp.to_account_uuid();
		std::cout << "Public key: " << kp.get_public_key() << std::endl;
	} else {
		palscash::key_pair kp(private_key);
		public_address = kp.to_account_uuid();
		std::cout << "Public key: " << kp.get_public_key() << std::endl;
	}

	std::cout << "Private key: " << private_key << std::endl;
	std::cout << "Public address: " << public_address << std::endl;

	// read template image into memory
	assets::surface_asset template_surface("data/template.png");

	// draw qr code of address into template image
	{

		// generate qr code fo public address
		auto qr_code = qrcodegen::QrCode::encodeText(public_address.c_str(), qrcodegen::QrCode::Ecc::LOW);

		assets::surface_asset surface_qr_code_public_address(350, 350);

		draw_qr_code(qr_code, surface_qr_code_public_address);

		auto x = (template_surface.get_surface()->w - surface_qr_code_public_address.get_surface()->w) / 2;

		SDL_Rect rect { x, 820, 0, 0 };

		SDL_BlitSurface(surface_qr_code_public_address.get_surface(), nullptr, template_surface.get_surface(), &rect);

	}

	// draw public address as string to template image
	{

		assets::font_asset address_font("data/DejaVuSansMono-Bold.ttf", 50);

		SDL_Color color_black { 0, 0, 0, SDL_ALPHA_OPAQUE };
		auto surface_public_address = address_font.generate_text(public_address, color_black);

		auto x = (template_surface.get_surface()->w - surface_public_address->get_surface()->w) / 2;

		SDL_Rect rect { x, 1325, 0, 0 };
		SDL_BlitSurface(surface_public_address->get_surface(), nullptr, template_surface.get_surface(), &rect);

	}

	// draw private key as string to template image
	{

		assets::font_asset address_font("data/DejaVuSansMono-Bold.ttf", 20);

		SDL_Color color_black { 99, 100, 96, SDL_ALPHA_OPAQUE };
		auto surface_private_key = address_font.generate_text(private_key, color_black);

		auto x = (template_surface.get_surface()->w - surface_private_key->get_surface()->w) / 2;

		SDL_Rect rect { x, 572, 0, 0 };
		SDL_BlitSurface(surface_private_key->get_surface(), nullptr, template_surface.get_surface(), &rect);

	}

	// show "save as" dialog

	// Save paper wallet image
	std::string file_name = public_address + ".png";
	IMG_SavePNG(template_surface.get_surface(), file_name.c_str());

	std::cout << "Saved to: " + file_name;

	// suggest to open after save/copy to clipboard

	return EXIT_SUCCESS;

}
