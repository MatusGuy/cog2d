// SPDX-License-Identifier: LGPL-3.0-only

// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <map>

#include <SDL2/SDL.h>

#include "cog2d/util/namespacedef.hpp"

COG2D_NAMESPACE_BEGIN_DECL

#define COG2D_CONTROLLER_TYPE(t) \
public:                          \
	static uint8_t type()        \
	{                            \
		return t;                \
	}                            \
	uint8_t get_type() override  \
	{                            \
		return t;                \
	}

struct InputAction;

class Controller
{
public:
	// unused for now
	uint8_t m_id = 0;

public:
	Controller();

	void apply_action(InputAction* action);
	void apply_finish();

	virtual void event(SDL_Event* ev) = 0;

	// 0 is reserved. These functions are meant to be overridden.
	static uint8_t type() { return 0; }
	virtual uint8_t get_type() = 0;

	bool held(uint8_t id) const;

protected:
	std::map<int, uint8_t> m_actions;
	std::map<uint8_t, bool> m_held;
};

COG2D_NAMESPACE_END_DECL
