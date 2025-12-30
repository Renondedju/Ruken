#pragma once

#include "Core/Types/FundamentalTypes.hpp"
#include "Core/Types/StrongType/StrongType.hpp"

BEGIN_RUKEN_NAMESPACE

class Archetype;

/// @brief 64 bit int component fingerprint
struct ComponentID: StrongType<RkSize, ComponentID>
{};

/// @brief Base component class
struct Component
{
    /// @returns the static class ID of the inheriting component.
    [[nodiscard]] ComponentID StaticID(this auto&&) noexcept
    {
        static ComponentID id {s_next_id++};
        return id;
    }

    protected:

        // Next component id
        static inline RkSize s_next_id {};
};

template<typename TType>
concept IsComponent = std::is_base_of_v<Component, TType>;

END_RUKEN_NAMESPACE
