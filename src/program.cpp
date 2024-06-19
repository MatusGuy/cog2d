#include "program.hpp"

#include <iostream>

#include "logger.hpp"

Program::Program():
	m_keep_running(true),
	m_paused(false)
{

}

int Program::run(int argc, char* argv[])
{
	std::atexit(&Program::quit);
	init_sdl();

	COG2D_USE_PAINTER;
	COG2D_USE_INPUTMANAGER;
	COG2D_USE_ACTORMANAGER;

	painter.init();
	if (!painter.get_error().empty()) {
		COG2D_CRASH("SDL", painter.get_error());
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
			for (Actor* actor : actormanager.get_actors()) {
				actor->update();
			}
		}

		draw();

		for (Actor* actor : actormanager.get_actors()) {
			actor->draw();
		}

		painter.update();
	}

	quit();

	return 0;
}

void Program::quit()
{
	COG2D_USE_PAINTER;
	COG2D_USE_ASSETMANAGER;

	assetmanager.wipe_assets();

	if (painter.get_error().empty())
		painter.deinit();

	SDL_Quit();
}

void Program::init_sdl() {
	int errcode = SDL_Init(SDL_INIT_EVERYTHING);
	if (errcode != 0) {
		std::stringstream stream;
		stream << SDL_GetError() << " (" << errcode << ")";
		COG2D_CRASH("SDL", stream.str());
	}
}

void Program::poll_sdl_events()
{
	COG2D_USE_INPUTMANAGER;

	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		if (ev.type == SDL_QUIT) {
			m_keep_running = false;
			break;
		}

		if (!event(&ev)) {
			continue;
		}

		inputmanager.event(&ev);
	}
}
