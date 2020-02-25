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

#include <memory>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

#include "ECS/Group.hpp"
#include "ECS/EntityID.hpp"
#include "ECS/ComponentBase.hpp"
#include "Containers/UnorderedMap.hpp"
#include "ECS/ArchetypeFingerprint.hpp"

BEGIN_DAEMON_NAMESPACE

class Archetype
{
    protected:

        #pragma region Members
        
        UnorderedMap<DAEsize, std::unique_ptr<ComponentBase>> m_components;
        ArchetypeFingerprint                                  m_fingerprint;

        #pragma endregion 

    public:

        #pragma region Constructors

        Archetype()                         = default;
        Archetype(Archetype const& in_copy) = default;
        Archetype(Archetype&&      in_move) = default;
        ~Archetype()                        = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Gets the fingerprint of the archetype
         * \return Fingerprint
         */
        ArchetypeFingerprint const& GetFingerprint() const noexcept;

        /**
         * \brief Returns a component of the passed type stored in this archetype
         * \tparam TComponent Component to look for
         * \note Passing a component type that does not exists in this archetype will result in a crash
         * \return Found component
         */
        template<typename TComponent>
        TComponent& GetComponent() noexcept;

        /**
         * \brief Creates an entity in the archetype
         * \return The new ID of this entity.
         * \see EntityID for lifetime info
         */
        EntityID CreateEntity() noexcept;

        /**
         * \brief Returns the total count of entity stored in this archetype
         * \return Entities count
         */
        DAEsize EntitiesCount() const noexcept;

        /**
         * \brief Creates a components reference group
         * \tparam TComponents Components to include in the group
         * \return Newly created reference group
         */
        template <typename... TComponents>
        Group<TComponents...> CreateGroupReference() noexcept;

        /**
         * \brief Creates an archetype
         * \tparam TComponents Components to use for the archetype initialization
         * \return New archetype
         */
        template <typename... TComponents>
        static Archetype CreateArchetype() noexcept;

        #pragma endregion

        #pragma region Operators

        Archetype& operator=(Archetype const& in_copy) = default;
        Archetype& operator=(Archetype&&      in_move) = default;

        #pragma endregion
};

#include "ECS/Archetype.inl"

END_DAEMON_NAMESPACE
