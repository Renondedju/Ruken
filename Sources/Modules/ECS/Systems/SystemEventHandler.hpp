#pragma once

#include "ECS/Components/ComponentQuery.hpp"
#include "Core/JobSystem/Awaitables/SyncTask/SyncTask.hpp"

#include <functional>

BEGIN_RUKEN_NAMESPACE

struct Universe;

/// @brief Base system event handler class
struct SystemEventHandler
{
	#pragma region Lifetime

	explicit SystemEventHandler(ComponentQuery const& in_query) noexcept;
	SystemEventHandler			 (SystemEventHandler const&) = default;
	SystemEventHandler			 (SystemEventHandler&&)		 = default;
	SystemEventHandler& operator=(SystemEventHandler&&)      = delete;
	SystemEventHandler& operator=(SystemEventHandler const&) = delete;
	virtual ~SystemEventHandler()							 = default;

	#pragma endregion

	/// @brief Query function to check if a handler is runnable on a given archetype/universe pair.
	ComponentQuery const component_query {};

	/// @brief Acquires components locks and executes the handler.
	virtual SyncTask<> ScheduleExecution(Universe& in_universe, Archetype& in_archetype) noexcept = 0;
};

template <typename TType>
concept IsSystemEventHandler = std::is_base_of_v<SystemEventHandler, TType>;

END_RUKEN_NAMESPACE