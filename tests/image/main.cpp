#include <iostream>

#include <painter.hpp>
#include <assetmanager.hpp>

bool keep_running = true;

void init_sdl() {
	int errcode = SDL_Init(SDL_INIT_AUDIO);
	if (errcode != 0) {
		std::cerr << "An SDL error has occurred!: \"" << SDL_GetError() << "\" (" << errcode << ")" << std::endl;
		std::exit(errcode);
	}
}

void poll_sdl_events() {
	SDL_Event event;
	while(SDL_PollEvent(&event))
		if (event.type == SDL_QUIT) keep_running = false;
}

int main(int argc, char* argv[]) {
	init_sdl();

	Painter::get().init();
	if (!Painter::get().get_error().empty()) {
		std::cerr << "An SDL error has occurred while initializing the Painter!: \"" << Painter::get().get_error() << "\"" << std::endl;
		return 1;
	}

	// FIXME
	Texture* logo = AssetManager::get().load_image("/mnt/data/Programming/CPP/cog2d/assets/images/cog2d-512x512.png");
	if (logo == nullptr) {
		return 1;
	}

	while (keep_running) {
		poll_sdl_events();

		Painter::get().draw_texture({50,50, logo->get_size().x, logo->get_size().y}, logo);

		Painter::get().update();
	}

	AssetManager::get().wipe_assets();
	Painter::get().deinit();
	SDL_Quit();

	return 0;
}
