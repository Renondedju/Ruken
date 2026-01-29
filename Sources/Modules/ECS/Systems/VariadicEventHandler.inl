#pragma once

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

	// Wait here is needed to
	co_await ParallelFor(0uz, chunk_count, [&](RkSize const in_chunk_index) {
		return ProcessChunk(in_chunk_index, universe_awaitables, entity_storage_iterators);
	});

	co_return;
}

template<IsComponent ... TComponents>
SyncTask<> VariadicEventHandler<TComponents...>::ProcessChunk(
	RkSize const            in_chunk_index,
	UniverseAwaitables&     in_universe_awaitables,
	EntityStorageIterators& in_entity_storage_iterators) noexcept
{
	// Tuple of entity component lock awaitables
	EntityAwaitables entity_lock_awaiters {GetEntityStorageAwaiters(in_entity_storage_iterators)};
	ComponentAccess  component_accesses   {
		co_await WhenAll(std::tuple_cat(in_universe_awaitables, entity_lock_awaiters))
	};

	Execute(
		in_chunk_index,
		std::get<0>(component_accesses)->size(), // Chunk size
		component_accesses
	);
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
	return std::apply([&](auto... in_it) {
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
