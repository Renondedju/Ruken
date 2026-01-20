#pragma once

#include "Core/Types/FundamentalTypes.hpp"
#include "Core/Types/StrongType/StrongType.hpp"
#include "Core/Types/StrongType/Operators/Comparison.hpp"

#include <functional>

BEGIN_RUKEN_NAMESPACE

class Archetype;

/// @brief Base component class
struct Component {};

/// @brief 64 bit int component fingerprint
struct RUKEN_EMPTY_BASES ComponentID final:
    StrongType<RkSize, ComponentID>,
    Comparison
{
    using StrongType::StrongType;
};

template<typename TType>
concept IsComponent = std::is_base_of_v<Component, TType>;

struct ComponentIDFactory
{
    static inline RkSize s_next_id {};

    /// @returns the static class ID of the inheriting component.
    template <IsComponent TComponent>
    static ComponentID StaticID()
    {
        static ComponentID id(s_next_id++);
        return id;
    }
};

END_RUKEN_NAMESPACE

template <>
struct std::hash<RUKEN_NAMESPACE::ComponentID>
{
    size_t operator()(RUKEN_NAMESPACE::ComponentID const& in_value) const noexcept
    { return static_cast<RkSize>(in_value); }
};