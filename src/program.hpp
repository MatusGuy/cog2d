#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "types.hpp"

#include "graphicsengine.hpp"
#include "inputmanager.hpp"
#include "assetmanager.hpp"
#include "actormanager.hpp"
#include "collisionsystem.hpp"
#include "audioengine.hpp"
#include "config.hpp"

enum System : std::uint8_t {
	SYSTEM_VIDEO = 0x1,
	SYSTEM_INPUT = 0x2,
	SYSTEM_ASSET = 0x4,
	SYSTEM_ACTOR = 0x8,
	SYSTEM_SOUND = 0x16,
	SYSTEM_CONFIG = 0x32,

	SYSTEM_EVERYTHING = 0xFF
};

struct ProgramSettings {
	std::string_view title = "cog2d";

	// TODO: Maybe move these graphics related settings to a struct?
	int lwidth = 800, lheight = 600;
	int wwidth = lwidth, wheight = lheight;
	inline void set_size(int w, int h) {
		lwidth = w; lheight = h;
		wwidth = w; wheight = h;
	}

	uint32_t window_flags = SDL_WINDOW_RESIZABLE;
	uint32_t render_flags = SDL_RENDERER_ACCELERATED |
							SDL_RENDERER_PRESENTVSYNC |
							SDL_RENDERER_TARGETTEXTURE;
	SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND;
	std::string_view scale_quality = "best";
	bool vsync = true;
	bool batching = true;

	std::string org_name;
	std::string app_name;

	std::uint8_t systems = System::SYSTEM_EVERYTHING;
};

class Program
{
public:
	ProgramSettings* m_settings;

public:
	Program();

	int run(int argc, char* argv[]);

	void quit();

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual bool event(SDL_Event* ev) = 0;

	// Return false if you won't be using the InputManager.
	virtual bool register_actions() { return false; }

	// Return false if you won't be using the Config system.
	virtual bool register_settings() { return false; }

protected:
	bool m_keep_running;
	bool m_paused; // FIXME: Remove me!

private:
	void init_sdl();
	void poll_sdl_events();

};

#endif // PROGRAM_HPP
