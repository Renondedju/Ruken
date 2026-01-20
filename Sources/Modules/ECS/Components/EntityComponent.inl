#pragma once

#include "JobSystem/Awaitables/Primitives/ParallelForEach.hpp"

BEGIN_RUKEN_NAMESPACE

template<NonVoid TData>
EntityComponent<TData>::EntityComponent(RkSize const in_chunk_elements) noexcept:
    storage        {},
    chunk_elements {in_chunk_elements}
{}

template<NonVoid TData>
SyncTask<> EntityComponent<TData>::CreateEntities(RkSize const in_count) noexcept
{
    // TODO: LIST LOCK, THIS IS NOT SAFE
    // For simplicity, we are going to wait for every chunk at once
    RkSize                   remaining    {in_count};
    std::vector<ChunkAccess> chunk_access {co_await ParallelForeach(storage, [](SharedMutex<Chunk>& in_chunk) {
        return in_chunk.AsyncWrite();
    })};

    // Filling existing chunks
    for (ChunkAccess const& chunk: chunk_access)
        remaining -= FillChunk(chunk, remaining);

    // Creating new chunks if needed
    while (remaining > 0)
    {
        Chunk chunk {};
        chunk.reserve(chunk_elements);
        remaining -= FillChunk(co_await storage.emplace_back(std::move(SharedMutex<Chunk>{
            std::move(chunk)
        })).AsyncWrite(), remaining);
    }

    co_return;
}

/*
template<NonVoid TData>
SyncTask<> EntityComponent<TData>::DeleteEntity(RkSize const in_index) noexcept
{
    RkSize const chunk_index {in_index / chunk_elements};

    // TODO: LIST LOCK, THIS IS NOT SAFE

}
*/

template<NonVoid TData>
RkSize EntityComponent<TData>::FillChunk(ChunkAccess const& in_chunk, RkSize const in_count)
{
    RkSize const allocated {std::min(in_count, in_chunk->capacity() - in_chunk->size())};

    for (RkSize i {0}; i <= allocated; i++)
        in_chunk->emplace_back(TData{});

    return allocated;
}

END_RUKEN_NAMESPACE