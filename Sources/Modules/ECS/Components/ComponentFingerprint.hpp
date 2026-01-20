#pragma once

#include <functional>

#include "Core/Bitwise/SizedBitmask.hpp"
#include "ECS/Components/Component.hpp"

#define RUKEN_MAX_ECS_COMPONENTS 64

BEGIN_RUKEN_NAMESPACE

/**
 * @brief Stores a bitmask holding data about the types of a group of component.
 *        This allows for fast archetype comparisons and fast component queries. 
 */
struct ComponentFingerprint: SizedBitmask<RUKEN_MAX_ECS_COMPONENTS / sizeof(RkSize) * 8, RkSize>
{
    /// @brief Creates a new fingerprint and setups traits based on the passed components
    template <IsComponent... TComponents>
    static ComponentFingerprint CreateFingerPrintFrom() noexcept;
};

END_RUKEN_NAMESPACE

#include "ECS/Components/ComponentFingerprint.inl"

#pragma region std::hash

// std::hash specialization for ArchetypeFingerprint
template <>
struct std::hash<RUKEN_NAMESPACE::ComponentFingerprint>
{
    size_t operator()(RUKEN_NAMESPACE::ComponentFingerprint const& in_key) const noexcept
    { return in_key.HashCode(); }
};

#pragma endregion