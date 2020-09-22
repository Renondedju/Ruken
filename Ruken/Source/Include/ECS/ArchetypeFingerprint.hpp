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

#include "Build/Config.hpp"
#include "Build/Namespace.hpp"

#include "Meta/MinimumType.hpp"
#include "ECS/ComponentBase.hpp"
#include "Bitwise/SizedBitmask.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Stores a bitmask holding data about the component types stored inside an archetype
 *        This allows for fast archetype comparisons and fast component queries. 
 */
class ArchetypeFingerprint : public SizedBitmask<RUKEN_MAX_ECS_COMPONENTS / 64, MinimumTypeT<RUKEN_MAX_ECS_COMPONENTS, RkSize>>
{
    public:

        #pragma region Constructors

        ArchetypeFingerprint()                                    = default;
        ArchetypeFingerprint(ArchetypeFingerprint const& in_copy) = default;
        ArchetypeFingerprint(ArchetypeFingerprint&&      in_move) = default;
        ~ArchetypeFingerprint()                                   = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Creates a new fingerprint and setups traits based on the passed components
         */
        template <ComponentType... TComponents>
        static ArchetypeFingerprint CreateFingerPrintFrom() noexcept;

        #pragma endregion

        #pragma region Operators

        ArchetypeFingerprint& operator=(ArchetypeFingerprint const& in_copy) = default;
        ArchetypeFingerprint& operator=(ArchetypeFingerprint&&      in_move) = default;

        #pragma endregion
};

#include "ECS/ArchetypeFingerprint.inl"

END_RUKEN_NAMESPACE

// std::hash specialization for ArchetypeFingerprint
namespace std
{
    template <>
    struct hash<RUKEN_NAMESPACE::ArchetypeFingerprint>
    {
        size_t operator()(RUKEN_NAMESPACE::ArchetypeFingerprint const& in_key) const noexcept
        {
            return in_key.HashCode();
        }
    };
}