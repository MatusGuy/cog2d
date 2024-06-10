#include <iostream>
#include <painter.hpp>

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

	while (keep_running) {
		poll_sdl_events();

		Painter::get().draw_rect({10, 10, 100, 100}, true);
		Painter::get().draw_rect({120, 10, 100, 100}, true, 0xFF0000FF);
		Painter::get().draw_rect({240, 10, 100, 100}, true, 0x00FFFFFF);

		Painter::get().draw_circle({60, 170}, 50, true);

		// Marty
		Painter::get().draw_rect({300, 300, 100, 200}, true, 0xFF0000FF);
		Painter::get().draw_rect({400, 300, 100, 200}, true, 0xFFFFFFFF);
		Painter::get().draw_circle({350, 400}, 25, true, 0xFFFFFFFF);
		Painter::get().draw_circle({450, 400}, 25, true, 0xFF0000FF);

		Painter::get().update();
	}

	Painter::get().deinit();
	SDL_Quit();

	return 0;
}
