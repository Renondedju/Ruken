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
#include <unordered_map>

#include "Config.hpp"

#include "ECS/EntityID.hpp"
#include "ECS/Archetype.hpp"
#include "ECS/ArchetypeBase.hpp"
#include "ECS/ComponentSystemBase.hpp"

BEGIN_RUKEN_NAMESPACE

class EntityAdmin
{
    private:

        #pragma region Members

        std::vector       <ComponentSystemBase*>                 m_systems;
        std::unordered_map<ArchetypeFingerprint, ArchetypeBase*> m_archetypes;
        
        #pragma endregion 

    public:

        #pragma region Constructors

        EntityAdmin()                           = default;
        EntityAdmin(EntityAdmin const& in_copy) = default;
        EntityAdmin(EntityAdmin&&      in_move) = default;
        ~EntityAdmin();

        #pragma endregion

        #pragma region Methods

        /**
         * \brief 
         * \tparam TSystem System type to push to the entity admin 
         */
        template <typename TSystem>
        DAEvoid CreateSystem() noexcept;

        /**
         * \brief Updates every system
         */
        DAEvoid UpdateSystems() noexcept;

        /**
         * \brief 
         * \tparam TComponents 
         * \return 
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

END_RUKEN_NAMESPACE