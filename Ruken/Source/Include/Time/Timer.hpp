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

#include "Types/Unique.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class Timer : Unique
{
    private:

        RkVoid* m_win_handle;

    public:

        #pragma region Constructors

        Timer ()                     noexcept;
        Timer (Timer const& in_copy) noexcept = delete;
        Timer (Timer&&      in_move) noexcept = delete;
        ~Timer()                     noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Sets the timer timing in nanoseconds
         * \param in_nanoseconds Amount of nanoseconds to set
         * \return true if the operation succeeded, false otherwise
         */
        RkBool SetTiming(RkInt64 in_nanoseconds) const noexcept;

        /**
         * \brief Sleeps for x amount of nanoseconds (set via SetTiming())
         * \return true if the operation succeeded, false otherwise
         */
        RkBool NSleep() const noexcept;

        #pragma endregion

        #pragma region Operators

        Timer& operator=(Timer const& in_copy) noexcept = delete;
        Timer& operator=(Timer&&      in_move) noexcept = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE