#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SDL_events.h>

#include "types.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class Screen
{
public:
	virtual ~Screen() {}

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual bool event(SDL_Event* ev) = 0;
};

COG2D_NAMESPACE_END_DECL

#endif  // SCREEN_HPP
