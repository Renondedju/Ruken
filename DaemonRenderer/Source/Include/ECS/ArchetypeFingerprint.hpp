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
#include "Bitwise/Bitmask.hpp"
#include "Containers/Vector.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

class ArchetypeFingerprint
{
    private:

        enum class EFragmentContent : DAEsize
        {};

        #pragma region Members

        Vector<Bitmask<EFragmentContent>> m_fingerprint;

        #pragma endregion

    public:

        #pragma region Constructors

        ArchetypeFingerprint()                                    = default;
        ArchetypeFingerprint(ArchetypeFingerprint const& in_copy) = default;
        ArchetypeFingerprint(ArchetypeFingerprint&&      in_move) = default;
        ~ArchetypeFingerprint()                                   = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Adds a trait to the current fingerprint. Duplicates will be skipped
         * \param in_trait Trait to add
         */
        DAEvoid AddTrait(DAEsize in_trait) noexcept;

        /**
         * \brief Checks if this fingerprint is a subset of another one
         * \param in_other Other fingerprint
         * \return True if this fingerprint is a subset of the other one, false otherwise
         */
        DAEbool IsSubsetOf(ArchetypeFingerprint const& in_other) const noexcept;

        #pragma endregion

        #pragma region Operators

        ArchetypeFingerprint& operator=(ArchetypeFingerprint const& in_copy) = default;
        ArchetypeFingerprint& operator=(ArchetypeFingerprint&&      in_move) = default;

        #pragma endregion
};

END_DAEMON_NAMESPACE