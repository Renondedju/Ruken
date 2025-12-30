#pragma once

template <NonVoid TData>
RkSize EntityComponent<TData>::EnsureStorageSpace(RkSize const in_size) noexcept
{
    // Computes the chunk id required to store all the requested data
    RkSize const chunk_id      { in_size / storage.chunk_element_count };
    RkSize const chunk_missing { chunk_id + 1 - storage.GetSize() };

    // If the size isn't big enough, allocating some more nodes to fit everything
    for (RkSize index = 0; index < chunk_missing; ++index)
        storage.CreateNode();

    // Returning the amount of elements created
    return static_cast<RkFloat>(chunk_missing * storage.chunk_element_count);
}
