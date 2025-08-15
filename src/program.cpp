// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "program.hpp"

#include <iostream>
#include <fstream>

#include <SDL2/SDL_ttf.h>

#include "cog2d/util/logger.hpp"
#include "cog2d/util/timer.hpp"
#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/audio/audioengine.hpp"
#include "cog2d/audio/musicplayer.hpp"
#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/config/config.hpp"
#include "cog2d/video/font/pixmapfont.hpp"

namespace cog2d {

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
	MusicPlayer::s_current = new MusicPlayer;
	AssetManager::s_current = new AssetManager;
	Config::s_current = new Config;

	COG2D_USE_GRAPHICSENGINE;
	COG2D_USE_INPUTMANAGER;
	COG2D_USE_AUDIOENGINE;
	COG2D_USE_ASSETMANAGER;

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

	TimePoint now = Clock::now();

	Timer timer;
	timer.start(100ms);

	while (m_keep_running) {
		m_prog_time = Clock::now();

		poll_sdl_events();

		if (timer.check() && true) {
			if (m_delta_time != Duration::zero())
				COG2D_LOG_DEBUG(fmt::format("FPS: {}, DT: {}, vel: {}", 1s / m_delta_time,
				                            m_delta_time, velocity_multiplier()));
			timer.start(100ms);
		}

		update_fonts_gc();
		MusicPlayer::get().update();

		std::unique_ptr<Screen>& screen = m_screen_stack.top();
		screen->update();

		graphicsengine.pre_draw();
		screen->draw();
		graphicsengine.post_draw();

		now = Clock::now();
		m_delta_time = now - m_prog_time;
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
		std::ofstream cfgfile(config.get_config_path(m_settings));
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

void Program::update_fonts_gc()
{
	COG2D_USE_ASSETMANAGER;

	// TODO: ttf fonts
	const AssetRefs<PixmapFont>& pfonts = assetmanager.pixmapfonts.get_assets();
	for (auto it = pfonts.begin(); it != pfonts.end(); ++it) {
		AssetRef<PixmapFont> pfont = it->second;
		std::shared_ptr<PixmapFont> pfont_ptr = pfont.lock();
		pfont_ptr->m_cache.update_gc();
	}
}

}  //namespace cog2d
