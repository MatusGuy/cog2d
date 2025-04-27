#ifndef KEYBOARDCONTROLLER_HPP
#define KEYBOARDCONTROLLER_HPP

#include "cog2d/input/controller.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class KeyboardController : public Controller
{
	COG2D_CONTROLLER_TYPE(1)

public:
	KeyboardController();

	void event(SDL_Event* ev) override;
};

COG2D_NAMESPACE_END_DECL

#endif  // KEYBOARDCONTROLLER_HPP
