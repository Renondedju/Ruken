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

/**
 * \brief Allows to call a method on a pointer safely
 * \param in_instance instance to call the method onto
 */
#define RUKEN_SAFE_POINTER_CALL(in_instance, ...) if (in_instance != nullptr) { in_instance->__VA_ARGS__; }

/**
 * \brief Safely calls a logging instance
 * \param in_instance logger
 * \note This macro will automatically optimize out logger calls if logging has been disabled in build
 */
#if defined(RUKEN_LOGGING_ENABLED)
    #define RUKEN_SAFE_LOGGER_CALL(in_instance, ...) RUKEN_SAFE_POINTER_CALL(in_instance, __VA_ARGS__)
#else
    #define RUKEN_SAFE_LOGGER_CALL(in_instance, ...)
#endif