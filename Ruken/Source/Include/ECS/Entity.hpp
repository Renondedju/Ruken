/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma once

#include "Config.hpp"
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

        Archetype* m_archetype        {nullptr};
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
         * \brief Returns the owning archetype of the entity
         * \return Owning archetype reference
         */
        Archetype const& GetOwner() const noexcept;

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

        Entity& operator=(Entity const& in_copy) = default;
        Entity& operator=(Entity&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE