
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class Archetype;

/**
 * \brief An entity only stores a reference onto its owning archetype and its local identifier, effectively making it a handle
 *        To fetch the data of any component attached to the entity, you must query the owning archetype
 */
class Entity
{
    private:

        #pragma region Members

        Archetype& m_archetype;
        RkSize     m_local_identifier {0ULL};

        #pragma endregion

    public:

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_archetype Archetype of the entity
         * \param in_local_identifier Unique local identifier of the entity
         */
        Entity(Archetype& in_archetype, RkSize in_local_identifier);
    
        Entity(Entity const& in_copy) = default;
        Entity(Entity&&      in_move) = default;
        ~Entity()                     = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Destroys the entity
         * \note This class is a handle, and thus won't be invalidated on an entity destruction
         */
        RkVoid Delete() const noexcept;

        /**
         * \brief Returns the owning archetype of the entity
         * \return Owning archetype reference
         */
        Archetype& GetOwner() const noexcept;

        /**
         * \brief Returns the unique local identifier of the entity
         * \return Local identifier
         * \note This can only be used to compare entities stored in the same archetype
         */
        RkSize GetLocalIdentifier() const noexcept;

        #pragma endregion

        #pragma region Operators

        /**
         * \brief Equality operator
         * \param in_other Other entity
         * \return True if the 2 compared entities are the same, false otherwise
         */
        RkBool operator==(Entity const& in_other) const noexcept;

        Entity& operator=(Entity const& in_copy) = delete;
        Entity& operator=(Entity&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE