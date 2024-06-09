
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Component counter class, should be used as a base of any components that seeks a unique ID in conjunction with the
 *        RUKEN_DEFINE_COMPONENT_ID_DECLARATION macro to allow the retrieval of the desired identifier.
 * \note The distributed identifiers are defined at static time, meaning that this implementation does not guarantees a deterministic
 *        distribution, but allows it if needed
 */
class ComponentCounter
{
    protected:

        #pragma region Members

        // Static component counter
        inline static RkSize m_id_counter {0ULL};

        #pragma endregion

    public:

        #pragma region Constructors

        ComponentCounter()                                = default;
        ComponentCounter(ComponentCounter const& in_copy) = default;
        ComponentCounter(ComponentCounter&&      in_move) = default;
        virtual ~ComponentCounter()                       = default;

        #pragma endregion

        #pragma region Operators

        ComponentCounter& operator=(ComponentCounter const& in_copy) = default;
        ComponentCounter& operator=(ComponentCounter&&      in_move) = default;

        #pragma endregion
};

/**
 * \brief Generates the code required to create a unique ID for any component
 */
#define RUKEN_DEFINE_COMPONENT_ID_DECLARATION inline static RkSize GetId() noexcept { static RkSize id = m_id_counter++; return id; }

END_RUKEN_NAMESPACE