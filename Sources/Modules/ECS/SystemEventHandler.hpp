#pragma once

#include "ECS/Universe.hpp"
#include "ECS/ComponentFingerprint.hpp"
#include "Core/JobSystem/Awaitables/AsyncTask/DynamicTask.hpp"

#include <functional>

BEGIN_RUKEN_NAMESPACE

/// @brief System event handler
struct SystemEventHandler
{
	/// @brief Query function to check if a handler is runnable on a given archetype/universe pair.
	std::function<RkBool(
		ComponentFingerprint const& in_universe,
		ComponentFingerprint const& in_archetype
	)> const component_query {};

	/// @brief Acquires components locks and executes the handler.
	RkVoid ScheduleExecution(Universe& in_universe, Archetype& in_archetype) noexcept;

    protected:

		/// @brief Executes the event handler for each matching archetype.
        virtual DynamicTask<> Execute(Universe& in_universe, Archetype& in_archetype) noexcept = 0;
};

END_RUKEN_NAMESPACE