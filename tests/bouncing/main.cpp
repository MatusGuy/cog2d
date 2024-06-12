#include <iostream>
#include <painter.hpp>
#include <actormanager.hpp>
#include <inputmanager.hpp>

#include "ball.hpp"

bool keep_running = true;

void init_sdl() {
	int errcode = SDL_Init(SDL_INIT_AUDIO);
	if (errcode != 0) {
		std::cerr << "An SDL error has occurred!: \"" << SDL_GetError() << "\" (" << errcode << ")" << std::endl;
		std::exit(errcode);
	}
}

void poll_sdl_events() {
	SDL_Event ev;
	while(SDL_PollEvent(&ev))
	{
		if (ev.type == SDL_QUIT)
		{
			keep_running = false;
			break;
		}

		InputManager::get().event(&ev);
	}
}

int main(int argc, char* argv[]) {
	init_sdl();

	Painter::get().init();
	if (!Painter::get().get_error().empty()) {
		std::cerr << "An SDL error has occurred while initializing the Painter!: \"" << Painter::get().get_error() << "\"" << std::endl;
		return 1;
	}

	auto ball = ActorManager::get().create<Ball>();

	while (keep_running) {
		poll_sdl_events();

		ball->update();
		ball->draw();

		Painter::get().update();
	}

	Painter::get().deinit();
	SDL_Quit();

	return 0;
}
