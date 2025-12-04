#include "Core/Time/Clock.hpp"

USING_RUKEN_NAMESPACE

Clock::Clock(ServiceProvider& in_service_provider) noexcept:
	Service		     {in_service_provider, typeid(Clock)},
	m_clock_start    {std::chrono::high_resolution_clock::now()}
{}

Seconds Clock::Tick() noexcept
{
	auto const now {std::chrono::high_resolution_clock::now()};

	m_last_tick_time = now - m_tick_start;
	m_tick_start     = now;

	return LastTickDuration();
}

Seconds Clock::LastTickDuration() const noexcept
{
	return Seconds {
		std::chrono::duration_cast<std::chrono::milliseconds>(
			m_last_tick_time
		).count() / 1000.0f
	};
}

Seconds Clock::TimeSinceCreation() const noexcept
{
	return Seconds {
		std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now() - m_clock_start
		).count() / 1000.0f
	};
}
