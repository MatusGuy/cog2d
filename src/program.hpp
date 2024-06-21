#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "painter.hpp"
#include "inputmanager.hpp"
#include "assetmanager.hpp"
#include "actormanager.hpp"
#include "soundengine.hpp"

class Program
{
public:
	Program();

	int run(int argc, char* argv[]);

	static void quit();

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual bool event(SDL_Event* ev) = 0;

	// Return false if you won't be using the InputManager.
	virtual bool register_actions() { return false; }

protected:
	bool m_keep_running;
	bool m_paused;

private:
	void init_sdl();
	void poll_sdl_events();

};

#endif // PROGRAM_HPP
