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
        remaining -= FillChunk(remaining, chunk);

    // Creating new chunks if needed
    while (remaining > 0)
        remaining -= FillChunk(remaining, co_await storage.emplace_back(SharedMutex<Chunk>{}).AsyncWrite());

    co_return;
}

template<NonVoid TData>
RkSize EntityComponent<TData>::FillChunk(RkSize const in_count, ChunkAccess const& in_chunk)
{
    if (in_chunk->capacity() != chunk_elements)
        in_chunk->reserve(chunk_elements);

    RkSize const allocated {std::min(in_count, in_chunk->capacity() - in_chunk->size())};

    for (RkSize i {0}; i <= allocated; i++)
        in_chunk->emplace_back(TData{});

    return allocated;
}

END_RUKEN_NAMESPACE