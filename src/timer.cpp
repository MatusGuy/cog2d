#include "timer.hpp"

Timer::Timer()
    : m_start(0),
      m_period(0)
{
}

void Timer::start(uint32_t period)
{
	COG2D_USE_PROGRAM;
	m_period = period;
	m_start = program.m_prog_time;
}

bool Timer::check()
{
	if (m_period == 0)
		return false;

	COG2D_USE_PROGRAM;
	if (program.m_prog_time - m_start >= m_period) {
		m_period = 0;
		return true;
	}

	return false;
}
