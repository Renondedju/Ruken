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

#include "Build/Namespace.hpp"

#include "ECS/ComponentQuery.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class SystemBase
{
    protected:

        #pragma region Mebers

        ComponentQuery m_query {};

        #pragma endregion

    public:

        #pragma region Members

        RkBool enabled {true};

        #pragma endregion

        #pragma region Constructors

        SystemBase()                          = default;
        SystemBase(SystemBase const& in_copy) = default;
        SystemBase(SystemBase&&      in_move) = default;
        virtual ~SystemBase()                 = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Returns the component query of the system
         * \return Component query
         */
        ComponentQuery const& GetQuery() const noexcept;

        // --- Virtual

        /**
         * \brief Adds a component reference group to the system.
         *        This is called by the entity admin at the creation of a new archetype
         *        if the component query of this archetype and the system matches
         * \param in_archetype Referenced archetype of the group to create 
         */
        virtual RkVoid AddReferenceGroup(Archetype& in_archetype) noexcept = 0;

        /**
         * \brief Called once at the start of a simulation
         * \note This method could be called multiple times for the same
         *       instance if the simulation is restated without reloading the whole ECS 
         */
        virtual RkVoid OnStart() noexcept;

        /**
         * \brief Called every frame
         */
        virtual RkVoid OnUpdate() noexcept;

        /**
         * \brief Called once at the end of a simulation
         * \note This method could be called multiple times for the same
         *       instance if the simulation is restated without reloading the whole ECS 
         */
        virtual RkVoid OnEnd() noexcept;

        #pragma endregion

        #pragma region Operators

        SystemBase& operator=(SystemBase const& in_copy) = default;
        SystemBase& operator=(SystemBase&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE
