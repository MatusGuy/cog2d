// SPDX-License-Identifier: LGPL-3.0-only

#include "program.hpp"

#include <iostream>
#include <fstream>

#include <SDL2/SDL_ttf.h>

#include "cog2d/util/logger.hpp"
#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/audio/audioengine.hpp"
#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/config/config.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

Program::Program()
    : m_keep_running(true),
      m_paused(false),
      m_settings(new ProgramSettings),
      m_screen_stack()
{
}

int Program::run(int argc, char* argv[])
{
	Logger::s_current = new Logger;

	// std::atexit(&Program::quit);
	init_sdl();

	GraphicsEngine::s_current = new GraphicsEngine;
	InputManager::s_current = new InputManager;
	AudioEngine::s_current = new AudioEngine;
	AssetManager::s_current = new AssetManager;
	Config::s_current = new Config;

	COG2D_USE_GRAPHICSENGINE;
	COG2D_USE_INPUTMANAGER;
	COG2D_USE_AUDIOENGINE;

	graphicsengine.init(m_settings);
	if (!graphicsengine.get_error().empty()) {
		COG2D_LOG_FATAL("SDL", graphicsengine.get_error());
	}

	audioengine.init();
	if (!audioengine.get_error().empty()) {
		COG2D_LOG_FATAL("SDL", audioengine.get_error());
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

	std::uint32_t now = SDL_GetTicks();

	while (m_keep_running) {
		poll_sdl_events();

		m_prog_time = SDL_GetTicks();
		m_delta_time = m_prog_time - now;

		std::unique_ptr<Screen>& screen = m_screen_stack.top();
		screen->update();

		graphicsengine.pre_draw();
		screen->draw();
		graphicsengine.post_draw();

		now = SDL_GetTicks();
	}

	quit();

	return 0;
}

void Program::quit()
{
	COG2D_USE_GRAPHICSENGINE;
	COG2D_USE_ASSETMANAGER;
	COG2D_USE_AUDIOENGINE;
	COG2D_USE_CONFIG;

	graphicsengine.deinit();

	if (m_settings->systems & System::SYSTEM_CONFIG) {
		File cfgfile(config.get_config_path(m_settings));
		cfgfile.open(File::OPENMODE_WRITE);
		config.save(cfgfile);
		cfgfile.close();
	}

	audioengine.deinit();

	TTF_Quit();
	SDL_Quit();
}

void Program::push_screen(std::unique_ptr<Screen> screen)
{
	Screen* screenptr = screen.get();
	m_screen_stack.push(std::move(screen));

	screenptr->init();
}

void Program::init_sdl()
{
	int errcode = SDL_Init(SDL_INIT_EVERYTHING);
	if (errcode != 0) {
		std::stringstream stream;
		stream << SDL_GetError() << " (" << errcode << ")";
		COG2D_LOG_FATAL("SDL", stream.str());
	}

	errcode = TTF_Init();
	if (errcode != 0) {
		std::stringstream stream;
		stream << SDL_GetError() << " (" << errcode << ")";
		COG2D_LOG_FATAL("SDL_ttf", stream.str());
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

		if (!m_screen_stack.top()->event(&ev)) {
			continue;
		}

		inputmanager.event(&ev);
	}
}

COG2D_NAMESPACE_END_IMPL
