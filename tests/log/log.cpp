#include "log.hpp"

#include <thread>

// FIXME: this doesnt compile on clang!?!?!?!?
#include <chrono>

#include <logger.hpp>

namespace cog2d_test {

void Log::init()
{
    using std::operator""s;

    COG2D_LOG_WARN("You have mere seconds");
    std::this_thread::sleep_for(2s);

	COG2D_LOG_INFO("3");
	std::this_thread::sleep_for(1s);
	COG2D_LOG_INFO("2");
	std::this_thread::sleep_for(1s);
	COG2D_LOG_INFO("1");
	std::this_thread::sleep_for(1s);

	COG2D_LOG_ERROR("Death", "You are ded. Not big surprise");
}

} // namespace cog2d_test
