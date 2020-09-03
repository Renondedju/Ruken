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

struct Sleep
{
    /**
     * \brief Native nanosecond sleep
     * \param in_nanoseconds nanoseconds to sleep for
     * \return true if the operation succeeded, false otherwise
     */
    static RkBool NsSleep(RkInt64 in_nanoseconds) noexcept;

    /**
     * \brief Guaranteed to work on every platform but might use more CPU and be less precise than a native sleep
     * \brief ~ 0.5 ms precision, 1 ms min sleep
     * \param in_seconds Seconds to sleep
     */
    static RkVoid StdSleep(RkDouble in_seconds) noexcept;
};

END_RUKEN_NAMESPACE