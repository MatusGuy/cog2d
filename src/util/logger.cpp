// SPDX-License-Identifier: LGPL-3.0-only

#include "logger.hpp"

namespace cog2d {

Logger::Logger()
{
}

void Logger::log(Level lv, const std::string& lvname, const std::string& cat,
                 const std::string& msg)
{
	std::ostream* stream = nullptr;
	if (lv == ERROR)
		stream = &std::cerr;
	else
		stream = &std::cout;

	*stream << "[" << get_color_code(lv) << lvname << "\033[0m] [" << cat << "] " << msg
	        << std::endl;
}

void Logger::log(Level lv, const std::string& lvname, const std::string& msg)
{
	std::ostream* stream = nullptr;
	if (lv == ERROR)
		stream = &std::cerr;
	else
		stream = &std::cout;

	*stream << "[" << get_color_code(lv) << lvname << "\033[0m] " << msg << std::endl;
}

std::string Logger::get_color_code(Level lv)
{
	switch (lv) {
	case INFO:
		return "\033[0;36m";

	case WARN:
		return "\033[0;33m";

	case ERROR:
		return "\033[1;31m";

	default:
		return "";
	}
}

}
