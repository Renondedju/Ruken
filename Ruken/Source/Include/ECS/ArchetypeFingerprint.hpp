
#pragma once

#include "Build/Config.hpp"
#include "Build/Namespace.hpp"

#include "Meta/MinimumType.hpp"
#include "Bitwise/SizedBitmask.hpp"
#include "Types/FundamentalTypes.hpp"

#include "ECS/Safety/AnyComponentType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Stores a bitmask holding data about the component types stored inside an archetype
 *        This allows for fast archetype comparisons and fast component queries. 
 */
class ArchetypeFingerprint : public SizedBitmask<RUKEN_MAX_ECS_COMPONENTS / 64, MinimumTypeT<RUKEN_MAX_ECS_COMPONENTS, RkSize>>
{
    public:

        #pragma region Constructors

        ArchetypeFingerprint()                                    = default;
        ArchetypeFingerprint(ArchetypeFingerprint const& in_copy) = default;
        ArchetypeFingerprint(ArchetypeFingerprint&&      in_move) = default;
        ~ArchetypeFingerprint()                                   = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Creates a new fingerprint and setups traits based on the passed components
         */
        template <AnyComponentType... TComponents>
        static ArchetypeFingerprint CreateFingerPrintFrom() noexcept;

        #pragma endregion

        #pragma region Operators

        ArchetypeFingerprint& operator=(ArchetypeFingerprint const& in_copy) = default;
        ArchetypeFingerprint& operator=(ArchetypeFingerprint&&      in_move) = default;

        #pragma endregion
};

#include "ECS/ArchetypeFingerprint.inl"

END_RUKEN_NAMESPACE

// std::hash specialization for ArchetypeFingerprint
namespace std
{
    template <>
    struct hash<RUKEN_NAMESPACE::ArchetypeFingerprint>
    {
        size_t operator()(RUKEN_NAMESPACE::ArchetypeFingerprint const& in_key) const noexcept
        {
            return in_key.HashCode();
        }
    };
}