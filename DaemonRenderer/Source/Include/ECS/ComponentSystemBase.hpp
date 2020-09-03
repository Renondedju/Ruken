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

#include "ECS/ComponentQuery.hpp"
#include "ECS/ArchetypeFingerprint.hpp"

BEGIN_RUKEN_NAMESPACE

class ComponentSystemBase
{
    private:

        #pragma region Mebers

        RkBool        m_enabled;
        ComponentQuery m_query;

        #pragma endregion

    protected:

        #pragma region Methods

        /**
         * \brief Sets up the target fingerprint of the system
         * \tparam TComponents Components of the system
         */
        template <typename... TComponents>
        RkVoid SetupTargetFingerprint() noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        ComponentSystemBase() noexcept;
        ComponentSystemBase(ComponentSystemBase const& in_copy) = default;
        ComponentSystemBase(ComponentSystemBase&&      in_move) = default;
        virtual ~ComponentSystemBase()                          = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Checks if the system is enabled 
         */
        RkBool Enabled() const;

        #pragma endregion

        #pragma region Operators

        ComponentSystemBase& operator=(ComponentSystemBase const& in_copy) = default;
        ComponentSystemBase& operator=(ComponentSystemBase&&      in_move) = default;

        #pragma endregion
};

#include "ECS/ComponentSystemBase.inl"

END_RUKEN_NAMESPACE
