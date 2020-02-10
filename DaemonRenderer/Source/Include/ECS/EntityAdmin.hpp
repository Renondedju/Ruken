/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

#include "Containers/Vector.hpp"

#include "ECS/ArchetypeBase.hpp"
#include "ECS/ComponentSystemBase.hpp"

BEGIN_DAEMON_NAMESPACE

class EntityAdmin
{
    private:

        #pragma region Members

        Vector<ComponentSystemBase> m_systems;
        Vector<ArchetypeBase>       m_archetypes;

        #pragma endregion 

    public:

        #pragma region Constructors

        EntityAdmin()                           = default;
        EntityAdmin(EntityAdmin const& in_copy) = default;
        EntityAdmin(EntityAdmin&&      in_move) = default;
        ~EntityAdmin()                          = default;

        #pragma endregion

        #pragma region Constructors

        /**
         * \brief 
         * \tparam TSystem System type to push to the entity admin 
         */
        template <typename TSystem>
        DAEvoid PushSystem() noexcept;

        
        DAEvoid PushArchetype() noexcept;

        /**
         * \brief Updates every system
         */
        DAEvoid UpdateSystems() noexcept;

        #pragma endregion

        #pragma region Operators

        EntityAdmin& operator=(EntityAdmin const& in_copy) = default;
        EntityAdmin& operator=(EntityAdmin&&      in_move) = default;

        #pragma endregion
};

#include "ECS/EntityAdmin.inl"

END_DAEMON_NAMESPACE