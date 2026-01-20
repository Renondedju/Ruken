#pragma once

#include "Core/JobSystem/Awaitables/Primitives/SharedMutex.hpp"
#include "Core/Types/Concepts/NonVoid.hpp"
#include "Core/Meta/IsBaseOfTemplate.hpp"

#include "ECS/Components/Component.hpp"

#include <list>
#include <vector>

#include "Components/ArchetypeComponent.hpp"

BEGIN_RUKEN_NAMESPACE

template <NonVoid TData>
struct EntityComponent: ArchetypeComponent
{
	// The whole point of data oriented programming is to optimize space and time locality
	// to properly utilize the cache and prefetcher. These 2 constants are used to make sure the data chunks
	// have a proper minimal size.
	static constexpr RkSize min_chunk_size		   = 1 << 15; // 2^15 = 32Kb
	static constexpr RkSize min_entities_per_chunk = min_chunk_size / sizeof(TData);

	using Chunk       = std::vector<TData>;
	using ChunkAccess = SharedMutex<Chunk>::WriteAccess;
	using Data		  = TData;

    #pragma region Lifetime

	explicit EntityComponent(RkSize in_chunk_elements) noexcept;
    EntityComponent& operator=(EntityComponent const&) = default;
    EntityComponent& operator=(EntityComponent&&     ) = default;
    EntityComponent           (EntityComponent const&) = default;
    EntityComponent           (EntityComponent&&     ) = default;
    ~EntityComponent() override						   = default;

    #pragma endregion

	SyncTask<> CreateEntities(RkSize in_count = 1) noexcept override;

	RkSize FillChunk(ChunkAccess const& in_chunk, RkSize in_count);

	/// @brief Data chunks
	std::list<SharedMutex<Chunk>> storage;
	RkSize const		   chunk_elements;
};

template <typename TType>
using EntityComponentType = IsBaseOfTemplate<EntityComponent, TType>;

template <typename TType>
concept IsEntityComponent = EntityComponentType<TType>::value;

END_RUKEN_NAMESPACE

#include "EntityComponent.inl"