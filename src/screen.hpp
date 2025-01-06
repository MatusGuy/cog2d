#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SDL_events.h>

class Screen
{
public:
	Screen();

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual bool event(SDL_Event* ev) = 0;
};

#endif // SCREEN_HPP
