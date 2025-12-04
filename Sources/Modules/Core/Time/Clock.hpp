#pragma once

#include "Core/Service.hpp"
#include "Core/Meta/Meta.hpp"

#include <chrono>

#include "Types/Units/Duration/Duration.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief Allows for time tracking in discrete intervals called ticks
struct Clock final : Service
{
	/// @brief Pretty name of the service.
	constexpr static std::string_view service_name = RUKEN_STRING(Timer);

	#pragma region Lifetime

	/**
	 * Default constructor.
	 * @param in_service_provider
	 */
	explicit Clock(ServiceProvider& in_service_provider) noexcept;
	 Clock           (Clock const& in_copy) = delete;
	 Clock           (Clock&&      in_move) = delete;
	 Clock& operator=(Clock const& in_copy) = delete;
	 Clock& operator=(Clock&&      in_move) = delete;
	~Clock() override                       = default;

	#pragma endregion

	#pragma region Methods

	/// @brief Marks the start of a new tick and returns the last tick duration
	Seconds Tick			 () noexcept;
	Seconds LastTickDuration () const noexcept;
	Seconds TimeSinceCreation() const noexcept;

	#pragma endregion

	private:

		#pragma region Members

		std::chrono::high_resolution_clock::time_point const m_clock_start;
		std::chrono::high_resolution_clock::time_point 		 m_tick_start	  {m_clock_start};
		std::chrono::high_resolution_clock::duration   		 m_last_tick_time {};

		#pragma endregion
};


END_RUKEN_NAMESPACE