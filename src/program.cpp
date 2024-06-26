#include "program.hpp"

#include <iostream>
#include <fstream>

#include "logger.hpp"

Program::Program():
	m_keep_running(true),
	m_paused(false),
	m_settings(new ProgramSettings)
{
}

int Program::run(int argc, char* argv[])
{
	//std::atexit(&Program::quit);
	init_sdl();

	COG2D_USE_PAINTER;
	COG2D_USE_INPUTMANAGER;
	COG2D_USE_ACTORMANAGER;
	COG2D_USE_SOUNDENGINE;

	painter.init();
	if (!painter.get_error().empty()) {
		COG2D_CRASH("SDL", painter.get_error());
	}

	soundengine.init();
	if (!soundengine.get_error().empty()) {
		COG2D_CRASH("SDL", soundengine.get_error());
	}

	if (register_actions()) {
		InputManager::get().init();
	}

	register_settings();
	if (m_settings->systems & System::SYSTEM_CONFIG) {
		Config::get().init(m_settings);
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
	COG2D_USE_SOUNDENGINE;
	COG2D_USE_CONFIG;

	if (m_settings->systems & System::SYSTEM_CONFIG) {
		std::ofstream cfgfile(config.get_config_path(m_settings));
		config.save(cfgfile);
		cfgfile.close();
	}

	assetmanager.wipe_assets();

	soundengine.deinit();

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
