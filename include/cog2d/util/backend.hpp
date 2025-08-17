// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <cinttypes>
#include <string>

#define COG2D_BACKEND(_b, _t, _n)           \
public:                                     \
	static std::string name_s()             \
    {                                       \
	    return #_t;                         \
	}                                       \
	std::string name() override             \
    {                                       \
	    return #_t;                         \
	}                                       \
	static std::string display_name_s()     \
    {                                       \
	    return _n;                          \
	}                                       \
	std::string display_name() override     \
    {                                       \
	    return _n;                          \
	}                                       \
	static std::uint8_t type_s()            \
    {                                       \
	    return ::cog2d::Backend::_b##_##_t; \
	}                                       \
	std::uint8_t type() override            \
    {                                       \
	    return ::cog2d::Backend::_b##_##_t; \
	}                                       \
	                                        \
private:

namespace cog2d {

class Backend
{
public:
	enum GraphicsEngine : std::uint8_t
	{
		GRAPHICS_SDL2
	};

	enum Pixmap : std::uint8_t
	{
		PIXMAP_SDL2_IMAGE
	};

public:
	virtual std::uint8_t type() { return 0; }
	virtual std::string name() { return ""; }
	virtual std::string display_name() { return ""; }
};

}  //namespace cog2d
