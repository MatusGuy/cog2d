#include "program.hpp"

#include <iostream>

Program::Program():
	m_keep_running(true),
	m_paused(false)
{

}

int Program::run(int argc, char* argv[])
{
	init_sdl();

	Painter::get().init();
	if (!Painter::get().get_error().empty()) {
		std::cerr << "An SDL error has occurred while initializing the Painter!: \"" << Painter::get().get_error() << "\"" << std::endl;
		return 1;
	}

	if (register_actions()) {
		InputManager::get().init();
	}

	// Run this after all essential systems
	// have been initialized.
	init();

	while (m_keep_running) {
		poll_sdl_events();

		update();

		if (!m_paused) {
			for (Actor* actor : ActorManager::get().get_actors()) {
				actor->update();
			}
		}

		draw();

		for (Actor* actor : ActorManager::get().get_actors()) {
			actor->draw();
		}

		Painter::get().update();
	}

	Painter::get().deinit();
	SDL_Quit();

	return 0;
}

void Program::init_sdl() {
	int errcode = SDL_Init(SDL_INIT_EVERYTHING);
	if (errcode != 0) {
		std::cerr << "An SDL error has occurred!: \"" << SDL_GetError() << "\" (" << errcode << ")" << std::endl;
		std::exit(errcode);
	}
}

void Program::poll_sdl_events()
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		if (ev.type == SDL_QUIT) {
			m_keep_running = false;
			break;
		}

		if (!event(&ev)) {
			continue;
		}

		InputManager::get().event(&ev);
	}
}
