// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "program.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <SDL2/SDL_ttf.h>

#include "cog2d/util/logger.hpp"
#include "cog2d/util/timer.hpp"
#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/audio/audioengine.hpp"
#include "cog2d/audio/musicplayer.hpp"
#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/video/font/pixmapfont.hpp"
#include "cog2d/audio/sdl2/sdl2audioengine.hpp"
#include "cog2d/audio/alsoft/alsoftaudioengine.hpp"

namespace cog2d {
Program s_program;

namespace program {

void init_sdl()
{
	int errcode = SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);
	if (errcode != 0) {
		std::stringstream stream;
		stream << SDL_GetError() << " (" << errcode << ")";
		log::fatal("SDL", stream.str());
	}

	errcode = TTF_Init();
	if (errcode != 0) {
		std::stringstream stream;
		stream << SDL_GetError() << " (" << errcode << ")";
		log::fatal("SDL_ttf", stream.str());
	}
}

void poll_sdl_events()
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		if (ev.type == SDL_QUIT) {
			s_program.keep_running = false;
			break;
		}

		if (!ext::event(&ev)) {
			continue;
		}

		if (!s_program.screen_stack.top()->event(&ev)) {
			continue;
		}

		input::event(&ev);
	}
}

void update_fonts_gc()
{
	// TODO: ttf fonts
	/*
	const AssetRefs<PixmapFont>& pfonts = cog2d::assets::pixmapfonts.get_assets();
	for (auto it = pfonts.begin(); it != pfonts.end(); ++it) {
		AssetRef<PixmapFont> pfont = it->second;
		std::shared_ptr<PixmapFont> pfont_ptr = pfont.lock();
		pfont_ptr->m_cache.update_gc();
	}
	*/
}

int run(int argc, char* argv[])
{
	ext::program_settings(s_program.settings);

	// std::atexit(&Program::quit);
	init_sdl();

	graphics::init(s_program.settings);

	audio::init(s_program.settings);
	audio::music.init();

	ext::register_actions();
	if (s_program.settings.systems & System::SYSTEM_INPUT) {
		input::init(s_program.settings);
	}

	ext::register_config();
	if (s_program.settings.systems & System::SYSTEM_CONFIG) {
		File file(std::filesystem::path(SDL_GetPrefPath(s_program.settings.org_name.data(),
		                                                s_program.settings.app_name.data())) /
		          "config.toml");
		file.open(File::OPENMODE_READ);
		toml::table config = toml::parse(file);
		file.close();
		ext::load_config(config);
	}

	// Run this after all essential systems
	// have been initialized.
	ext::init();

	TimePoint now = Clock::now();

	Timer timer;
	timer.start(100ms);

	while (s_program.keep_running) {
		s_program.prog_time = Clock::now();

		poll_sdl_events();

		if (timer.check() && false) {
			if (s_program.delta_time != Duration::zero())
				log::debug(fmt::format("FPS: {}, DT: {}, vel: {}", 1s / s_program.delta_time,
				                       s_program.delta_time, velocity_multiplier()));
			timer.start(100ms);
		}

		update_fonts_gc();
		//MusicPlayer::get().update();

		std::unique_ptr<Screen>& screen = s_program.screen_stack.top();
		screen->update();

		graphics::pre_draw();
		screen->draw();
		graphics::post_draw();

		now = Clock::now();
		s_program.delta_time = now - s_program.prog_time;
	}

	quit();

	return 0;
}

void quit()
{
	graphics::deinit();

	if (s_program.settings.systems & System::SYSTEM_CONFIG) {
		File file(std::filesystem::path(SDL_GetPrefPath(s_program.settings.org_name.data(),
		                                                s_program.settings.app_name.data())) /
		          "config.toml");
		file.open(File::OPENMODE_WRITE);
		toml::table config;
		ext::save_config(config);
		*file.stl_stream() << config;
		file.close();
	}

	audio::deinit();

	TTF_Quit();
	SDL_Quit();
}

void push_screen(std::unique_ptr<Screen> screen)
{
	Screen* screenptr = screen.get();
	s_program.screen_stack.push(std::move(screen));

	screenptr->init();
}

}  //namespace program
}  //namespace cog2d
