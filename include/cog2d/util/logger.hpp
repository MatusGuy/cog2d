// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>
#include <fmt/format.h>

#include "cog2d/util/currenton.hpp"

COG2D_NAMESPACE_BEGIN_DECL

#define COG2D_LOGGING_LEVEL(lv, name, func)                                \
	static constexpr uint8_t name = lv;                                    \
	inline void log_##func(const std::string& msg)                         \
	{                                                                      \
		log(lv, #name, msg);                                               \
	}                                                                      \
	inline void log_##func(const std::string& cat, const std::string& msg) \
	{                                                                      \
		log(lv, #name, cat, msg);                                          \
	}

#define COG2D_LOG(func) Logger::get().log_##func
#define COG2D_LOG_DEBUG COG2D_LOG(debug)
#define COG2D_LOG_INFO COG2D_LOG(info)
#define COG2D_LOG_WARN COG2D_LOG(warn)
#define COG2D_LOG_ERROR COG2D_LOG(error)
#define COG2D_LOG_FATAL COG2D_LOG(fatal)

#define COG2D_USE_LOGGER COG2D_USING(Logger, logger)
class Logger : public Currenton<Logger>
{
public:
	using Level = uint8_t;

	Logger();

	void log(Level lv, const std::string& lvname, const std::string& cat, const std::string& msg);
	void log(Level lv, const std::string& lvname, const std::string& msg);

	std::string get_color_code(Level lv);

	static constexpr uint8_t DEBUG = 0;
	inline void log_debug(const std::string& msg)
	{
#ifdef COG2D_DEBUG
		log(DEBUG, "DEBUG", msg);
#endif
	}
	inline void log_debug(const std::string& cat, const std::string& msg)
	{
#ifdef COG2D_DEBUG
		log(DEBUG, "DEBUG", cat, msg);
#endif
	}

	COG2D_LOGGING_LEVEL(1, INFO, info)
	COG2D_LOGGING_LEVEL(2, WARN, warn)
	COG2D_LOGGING_LEVEL(3, ERROR, error)

	inline void log_fatal(const std::string& cat, const std::string& msg, int exitcode = 1)
	{
		log_error(cat, msg);
		std::exit(exitcode);
	}

	inline void log_fatal(const std::string& msg, int exitcode = 1)
	{
		log_error(msg);
		std::exit(exitcode);
	}
};

COG2D_NAMESPACE_END_DECL
