#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdint>

#include "program.hpp"

class Timer
{
public:
	Timer();

	// TODO: timeframe/duration typedef
	void start(std::uint32_t period);
	bool check();

	/** returns the period of the timer or 0 if it isn't started */
	inline float get_period() const { return m_period; }
	inline float get_timeleft() const { return m_period - (Program::get().m_prog_time - m_start); }
	inline float get_timegone() const { return Program::get().m_prog_time - m_start; }
	inline float get_progress() const { return get_timegone() / get_period(); }
	inline bool started() const { return (m_period != 0 && get_timeleft() > 0); }

public:
	std::uint32_t m_start;
	std::uint32_t m_period;
};

#endif // TIMER_HPP
