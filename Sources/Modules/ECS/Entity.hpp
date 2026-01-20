#pragma once

#include "Core/Build/Namespace.hpp"
#include "Core/Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class Archetype;

/**
 * \brief An entity only stores a reference onto its owning archetype and its local identifier, effectively making it a handle
 *        To fetch the data of any component attached to the entity, you must query the owning archetype
 */
class Entity
{
    Archetype& m_archetype;
    RkSize     m_index {0ULL};

    public:

        #pragma region Constructors

        /**
         * @brief Default constructor
         * @param in_archetype Archetype of the entity
         * @param in_index Unique local identifier of the entity
         */
        Entity(Archetype& in_archetype, RkSize in_index);
        Entity			 (Entity const&) = default;
        Entity			 (Entity&&     ) = default;
		Entity& operator=(Entity const&) = delete;
		Entity& operator=(Entity&&     ) = delete;
        ~Entity()						 = default;

        #pragma endregion

        #pragma region Methods

        /**
         * @brief Destroys the entity
         * @note This class is a handle, and thus won't be invalidated on an entity destruction
         */
        RkVoid Delete() const noexcept;

        /**
         * @brief Returns the owning archetype of the entity
         * @return Owning archetype reference
         */
        Archetype& GetOwner() const noexcept;

        /**
         * @brief Returns the unique local identifier of the entity
         * @return Local index
         */
        RkSize GetIndex() const noexcept;

        #pragma endregion

        #pragma region Operators

        /**
         * @brief Equality operator
         * @param in_other Other entity
         * @return True if the 2 compared entities are the same, false otherwise
         */
        RkBool operator==(Entity const& in_other) const noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE