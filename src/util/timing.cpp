// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "timing.hpp"

#include "cog2d/program.hpp"

namespace cog2d {

float velocity_multiplier()
{
	//return static_cast<float>(Program::get().m_delta_time.count()) / LOGICAL_DT.count();
	return 1;
}

}  //namespace cog2d
