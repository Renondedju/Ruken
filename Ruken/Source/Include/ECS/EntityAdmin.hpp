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

#include <vector>
#include <memory>
#include <unordered_map>

#include "Build/Namespace.hpp"

#include "ECS/EntityID.hpp"
#include "ECS/Archetype.hpp"
#include "ECS/SystemBase.hpp"
#include "ECS/ExclusiveComponentBase.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief EntityAdmins are for isolation.
 *        Each admin can be described as a simulation containing entities
 *        and a group of system to maintain and update theses entities.
 */
class EntityAdmin
{
    private:

        #pragma region Members

        std::vector       <std::unique_ptr<SystemBase>>                      m_systems;
        std::unordered_map<ArchetypeFingerprint, std::unique_ptr<Archetype>> m_archetypes;
        std::unordered_map<DAEsize, std::unique_ptr<ExclusiveComponentBase>> m_exclusive_components;
        
        #pragma endregion 

        #pragma region Methods

        /**
         * \brief Creates a new archetype and handles any setup co-routine
         * \tparam TComponents 
         */
        template <typename... TComponents>
        Archetype* CreateArchetype() noexcept;

        #pragma endregion 

    public:

        #pragma region Constructors

        EntityAdmin()                           = default;
        EntityAdmin(EntityAdmin const& in_copy) = default;
        EntityAdmin(EntityAdmin&&      in_move) = default;
        ~EntityAdmin()                          = default;

        #pragma endregion

        #pragma region Methods

        // --- Simulation manipulation

        DAEvoid StartSimulation () noexcept;
        DAEvoid UpdateSimulation() noexcept;
        DAEvoid EndSimulation   () noexcept;

        // --- Entity / Systems lifetime manipulation

        /**
         * \brief Creates a system and adds it to the world
         * \tparam TSystem System type to push to the entity admin 
         */
        template <typename TSystem>
        DAEvoid CreateSystem() noexcept;

        /**
         * \brief Attempts the creation of an exclusive component
         *        If the component already existed, this method won't have any effects
         * \tparam TComponent Component class to add
         */
        template <typename TComponent>
        DAEvoid CreateExclusiveComponent() noexcept;

        /**
         * \brief Fetches an exclusive component
         * \tparam TComponent Exclusive component type to fetch
         * \return The component or nullptr if no component of this type has been found
         */
        template <typename TComponent>
        [[nodiscard]] TComponent* GetExclusiveComponent() noexcept;

        /**
         * \brief Creates a new entity with given components
         * \tparam TComponents Components to attach to the new entity
         * \return Created entity id
         */
        template <typename... TComponents>
        EntityID CreateEntity() noexcept;

        #pragma endregion

        #pragma region Operators

        EntityAdmin& operator=(EntityAdmin const& in_copy) = default;
        EntityAdmin& operator=(EntityAdmin&&      in_move) = default;

        #pragma endregion
};

#include "ECS/EntityAdmin.inl"

END_DAEMON_NAMESPACE
