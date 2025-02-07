#include "program.hpp"

#include <iostream>
#include <fstream>

#include "logger.hpp"
#include "graphicsengine.hpp"
#include "audioengine.hpp"
#include "assetmanager.hpp"
#include "config.hpp"

Program::Program():
	m_keep_running(true),
	m_paused(false),
	m_settings(new ProgramSettings),
	m_screen_stack()
{
}

int Program::run(int argc, char* argv[])
{
	Logger::s_current = new Logger;

	//std::atexit(&Program::quit);
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

	if (m_settings->systems & System::SYSTEM_CONFIG) {
		std::ofstream cfgfile(config.get_config_path(m_settings));
		config.save(cfgfile);
		cfgfile.close();
	}

	assetmanager.wipe_assets();

	audioengine.deinit();

	graphicsengine.deinit();

	TTF_Quit();
	SDL_Quit();
}

void Program::push_screen(Screen* screen)
{
	m_screen_stack.push(std::unique_ptr<Screen>(screen));

	screen->init();
}

void Program::init_sdl() {
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
