// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "controller.hpp"

#include <iostream>

#include "cog2d/input/inputmanager.hpp"
#include "cog2d/util/logger.hpp"

namespace cog2d {

Controller::Controller()
    : m_id(0),
      m_actions(),
      m_held()
{
}

void Controller::apply_action(InputAction* action)
{
	InputAction::Config* config = nullptr;

	// Find first config corresponding to controller type.
	for (auto conf : action->configs) {
		if (conf.controller_type == get_type()) {
			config = &conf;
			break;
		}
	}

	if (config == nullptr) {
		std::stringstream stream;
		stream << "Could not find config for controller type " << get_type();
		COG2D_LOG_ERROR("cog2d", stream.str());
		return;
	}

	m_actions[config->input_id] = action->id;
	m_held[action->id] = false;
}

void Controller::apply_finish()
{
}

bool Controller::held(uint8_t id) const
{
	auto it = m_held.find(id);
	if (it == m_held.end())
		return false;

	return it->second;
}

}
