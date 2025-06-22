// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <toml++/toml.hpp>

#include "cog2d/filesystem/iodevice.hpp"

namespace toml {
inline auto parse(cog2d::IoDevice&& stream)
{
	if (!stream.is_open())
		stream.open(cog2d::IoDevice::OPENMODE_READ);

	auto out = parse(*stream.stl_stream());

	stream.close();

	return out;
}
}  //namespace toml
