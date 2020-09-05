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
#include <unordered_map>

#include "Config.hpp"

#include "ECS/Group.hpp"
#include "ECS/Entity.hpp"
#include "ECS/ComponentBase.hpp"
#include "ECS/ArchetypeFingerprint.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Archetypes are at the very core of this ECS implementation.
 *        They are responsible for storing and organizing the components
 *        so that systems can query and work on those in the most efficient way.
 *
 * An archetype is a unique set of arrays, each array being used to store one (and one only) type of component.
 * All these arrays are ordered so that all components at a given index correspond to the same entity;
 * and so, all entities stored in a given archetype share the exact same set of components.
 * By storing the components in contiguous homogeneous arrays, the systems can iterate on them very efficiently,
 * leveraging the hardware pre-fetcher to its fullest potential.
 *
 * \note This has a very important implication: each time the structure of an entity is modified
 *       (i.e. each time we add or remove a component to an entity), it must be memmoved to another archetype.
 *       This has a cost, especially if doing this on lots of entities very frequently.
 *       Take this fact into consideration when designing your code.
 */
class Archetype
{
    protected:

        #pragma region Members
        
        std::unordered_map<DAEsize, std::unique_ptr<ComponentBase>> m_components  {};
        ArchetypeFingerprint                                        m_fingerprint {};

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
         * \return Entity handle.
         * \see Entity for lifetime info
         */
        Entity CreateEntity() noexcept;

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
