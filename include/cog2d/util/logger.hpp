// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <cstdio>

#include "cog2d/util/fmt.hpp"

namespace cog2d {

#define COG2D_LOGGING_LEVEL(_name, _func, _colorcode, _stream)                                     \
	inline void _func(const std::string& msg)                                                      \
    {                                                                                              \
	    std::fprintf(_stream, "[" _colorcode #_name "\033[0m] %s\n", msg.data());                  \
	    std::fflush(_stream);                                                                      \
    }                                                                                              \
	inline void _func(const std::string& cat, const std::string& msg)                              \
    {                                                                                              \
	    std::fprintf(_stream, "[" _colorcode #_name "\033[0m] [%s] %s\n", cat.data(), msg.data()); \
	    std::fflush(_stream);                                                                      \
    }

namespace log {

inline void debug(const std::string& msg)
{
#ifdef COG2D_DEBUG
	std::fprintf(stdout, "[DEBUG] %s\n", msg.data());
	std::fflush(stdout);
#endif
}
inline void debug(const std::string& cat, const std::string& msg)
{
#ifdef COG2D_DEBUG
	std::fprintf(stdout, "[DEBUG] [%s] %s\n", cat.data(), msg.data());
	std::fflush(stdout);
#endif
}

COG2D_LOGGING_LEVEL(INFO, info, "\033[0;36m", stdout)
COG2D_LOGGING_LEVEL(WARN, warn, "\033[0;33m", stdout)
COG2D_LOGGING_LEVEL(ERROR, error, "\033[1;31m", stderr)

inline void fatal(const std::string& cat, const std::string& msg, int exitcode = 1)
{
	error(cat, msg);
	std::exit(exitcode);
}

inline void fatal(const std::string& msg, int exitcode = 1)
{
	error(msg);
	std::exit(exitcode);
}

}  //namespace log
}  //namespace cog2d
