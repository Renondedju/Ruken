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

class Range
{
    public:

        #pragma region Members

        RkSize begin;
        RkSize size;

        #pragma endregion

        #pragma region Constructors

        /**
         * \brief Default constructor
         * \param in_begin Start index of the free range
         * \param in_size Size of the free range
         */
        Range(RkSize in_begin, RkSize in_size);

        Range(Range const& in_copy) = default;
        Range(Range&&      in_move) = default;
        ~Range()                    = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Reduces the range by one from the right
         * \return New size of the range
         */
        RkSize ReduceRight() noexcept;

        #pragma endregion

        #pragma region Operators

        Range& operator=(Range const& in_copy) = default;
        Range& operator=(Range&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE