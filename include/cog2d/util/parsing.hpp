// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <toml++/toml.hpp>

class cog2d::IoDevice;

namespace toml {
inline auto parse(cog2d::IoDevice& stream)
{
	return parse(*stream.stl_stream());
}
}  //namespace toml
