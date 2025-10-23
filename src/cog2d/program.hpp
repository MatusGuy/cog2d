// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <memory>
#include <stack>
#include <cstdint>
#include <chrono>

#include "cog2d/input/inputmanager.hpp"
#include "cog2d/util/toml.hpp"
#include "cog2d/screen.hpp"
#include "cog2d/util/typetraits.hpp"
#include "cog2d/util/timing.hpp"
#include "cog2d/util/currenton.hpp"

namespace cog2d {

#define COG2D_MAIN                                \
	int main(int argc, char* argv[])              \
    {                                             \
	    return ::cog2d::program::run(argc, argv); \
	}

enum System : std::uint8_t
{
	SYSTEM_VIDEO = 1 << 0,
	SYSTEM_INPUT = 1 << 1,
	SYSTEM_ASSET = 1 << 2,
	SYSTEM_SOUND = 1 << 3,
	SYSTEM_CONFIG = 1 << 4,

	SYSTEM_EVERYTHING = 0xFF
};

struct ProgramSettings
{
	std::string_view title = "cog2d";

	// TODO: Maybe move these graphics related settings to a struct?
	int lwidth = 800, lheight = 600;
	int wwidth = lwidth, wheight = lheight;
	inline void set_size(int w, int h)
	{
		lwidth = w;
		lheight = h;
		wwidth = w;
		wheight = h;
	}

	uint32_t window_flags = SDL_WINDOW_RESIZABLE;
	uint32_t render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
	                        SDL_RENDERER_TARGETTEXTURE;
	SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND;
	std::string_view scale_quality = "best";
	bool vsync = true;
	bool batching = true;
	bool proxy_texture = false;

	std::string org_name;
	std::string app_name;

	std::uint8_t systems = System::SYSTEM_EVERYTHING;
};

struct Program
{
	ProgramSettings settings;

	TimePoint prog_time;
	Duration delta_time;
	std::stack<std::unique_ptr<Screen>> screen_stack;
	bool keep_running = true;
};

extern Program s_program;
namespace program {
int run(int argc, char* argv[]);

void quit();

void push_screen(std::unique_ptr<Screen> screen);
void pop_screen();

template<class T>
inline T& get_screen_as()
{
	return *static_cast<T*>(s_program.screen_stack.top().get());
}

namespace ext {
void program_settings(ProgramSettings& settings);
void init();
bool event(SDL_Event* ev);

void load_config(const TomlTable& table);
void save_config(TomlTable& table);

// Return false if you won't be using the InputManager.
void register_actions();

// Return false if you won't be using the Config system.
void register_config();
}  //namespace ext
}  //namespace program
}  //namespace cog2d
