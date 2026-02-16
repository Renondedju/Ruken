#pragma once

#include "ECS/Systems/VariadicEventHandler.hpp"
#include "JobSystem/Awaitables/Primitives/ParallelFor.hpp"

BEGIN_RUKEN_NAMESPACE

template<IsComponent ... TComponents>
SyncTask<> VariadicEventHandler<TComponents...>::ScheduleExecution(
	Universe&  in_universe,
	Archetype& in_archetype) noexcept
{
	UniverseAwaitables	   universe_awaitables      {GetUniverseComponentAwaitables(in_universe)};
	EntityStorageIterators entity_storage_iterators {GetEntityStorageIterators     (in_archetype)};

	// Iterating over every chunk in entity component storage
	RkSize const chunk_size  {in_archetype.GetChunkSize()};
	RkSize const chunk_count {in_archetype.GetEntitiesCount() / chunk_size};

	co_await ParallelFor(0uz, chunk_count, [&](RkSize const in_chunk_index)
	{
		EntityAwaitables		    entity_awaitables {GetEntityStorageAwaiters(entity_storage_iterators)};
		SyncTask<ComponentAccess>&& component_access  {WhenAll(std::tuple_cat(
			std::move(universe_awaitables),
			std::move(entity_awaitables  )))
		};

		return ProcessChunk(in_chunk_index, std::move(component_access));
	});
}

template<IsComponent ... TComponents>
ECSTask<RkVoid> VariadicEventHandler<TComponents...>::ProcessChunk(
	RkSize const     		  in_chunk_index,
	SyncTask<ComponentAccess> in_component_access) noexcept
{
	ComponentAccess component_accesses {co_await    in_component_access};
	RkSize const    chunk_size 		   {std::get<0>(component_accesses)->size()};

	Execute(in_chunk_index, chunk_size, component_accesses);

	co_return;
}

// Various helpers hiding ugly variadic logic
template<IsComponent ... TComponents>
auto VariadicEventHandler<TComponents...>::GetEntityStorageIterators(Archetype& in_archetype)
	-> EntityStorageIterators
{
	return [&]<auto... TValues>(std::index_sequence<TValues...>) {
		return std::make_tuple(
			in_archetype.GetComponent<std::tuple_element_t<TValues, EntityComponents>>().storage.begin()...
		);
	}(std::make_index_sequence<std::tuple_size_v<EntityComponents>>());
}

template<IsComponent ... TComponents>
auto VariadicEventHandler<TComponents...>::GetEntityStorageAwaiters(EntityStorageIterators& in_iterators)
	-> EntityAwaitables
{
	return std::apply([&](auto&... in_it) {
		return std::make_tuple((in_it++)->AsyncAccess()...);
	}, in_iterators);
}

template<IsComponent ... TComponents>
auto VariadicEventHandler<TComponents...>::GetUniverseComponentAwaitables(Universe& in_universe) -> UniverseAwaitables
{
	return [&]<auto... TValues>(std::index_sequence<TValues...>) {
		return std::make_tuple(
			in_universe.GetUniverseComponent<std::tuple_element_t<TValues, UniverseAwaitables>>().data.AsyncAccess()...
		);
	}(std::make_index_sequence<std::tuple_size_v<UniverseAwaitables>>());
}

END_RUKEN_NAMESPACE
