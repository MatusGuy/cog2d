#ifndef KEYBOARDCONTROLLER_HPP
#define KEYBOARDCONTROLLER_HPP

#include <controller.hpp>

class KeyboardController : public Controller
{
	COG2D_CONTROLLER_TYPE(1)

public:
	KeyboardController();

	void event(SDL_Event* ev) override;
};

COG2D_NAMESPACE_END_DECL

#endif  // KEYBOARDCONTROLLER_HPP
