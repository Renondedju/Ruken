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

#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

enum class ELogLevel : DAEuint8
{
    /**
     * \brief Detailed information, typically of interest only when diagnosing problems.
     */
    Debug,

    /**
     * \brief Confirmation that things are working as expected.
     */
    Info,

    /**
     * \brief An indication that something unexpected happened, or indicative of some problem in the near future.
     * \note  The software is still working as expected.
     */
    Warning,

    /**
     * \brief Due to a more serious problem, the software has not been able to perform some function.
     */
    Error,

    /**
     * \brief A serious error, indicating that the program itself may be unable to continue running.
     */
    Fatal
};

END_DAEMON_NAMESPACE