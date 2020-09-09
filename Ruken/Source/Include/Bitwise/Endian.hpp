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

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

#define RUKEN_IS_BIG_ENDIAN    (*(RkUint16 *)"\0\1" == 1)
#define RUKEN_IS_LITTLE_ENDIAN !RUKEN_IS_BIG_ENDIAN

/**
 * \brief Swaps the endianness of the value (16).
 * \param in_x Base value
 * \return Converted value
 */
static RkUint16 Swap16Endian (RkUint16 const in_x)
{
    return in_x << 8 | in_x >> 8;
}

/**
 * \brief Swaps the endianness of the value (32).
 * \param in_x Base value
 * \return Converted value
 */
static RkUint32 Swap32Endian(RkUint32 const in_x)
{
    return in_x << 24 | in_x >> 24 |
		(in_x & 0xff00) << 8 | (in_x >> 8 & 0xff00);
}

/**
 * \brief Swaps the endianness of the value (64).
 * \param in_x Base value
 * \return Converted value
 */
static RkUint64 Swap64Endian(RkUint64 const in_x)
{
    return in_x << 56 | in_x >> 56 |
        (in_x & 0xff00    ) << 40 | (in_x >> 40 & 0xff00) |
        (in_x & 0xff0000  ) << 24 | (in_x >> 24 & 0xff0000) |
        (in_x & 0xff000000) << 8  | (in_x >>  8 & 0xff000000);
}

/**
 * \brief Converts a value to a big endian value (16).
 * \param in_x Base value
 * \return Converted value 
 */
static RkUint16 ToBigEndian16(RkUint16 const in_x)
{
    return RUKEN_IS_BIG_ENDIAN ? in_x : Swap16Endian(in_x);
}

/**
 * \brief Converts a value to a big endian value (32).
 * \param in_x Base value
 * \return Converted value 
 */
static RkUint32 ToBigEndian32(RkUint32 const in_x)
{
    return RUKEN_IS_BIG_ENDIAN ? in_x : Swap32Endian(in_x);
}

/**
 * \brief Converts a value to a big endian value (64).
 * \param in_x Base value
 * \return Converted value 
 */
static RkUint64 ToBigEndian64(RkUint64 const in_x)
{
    return RUKEN_IS_BIG_ENDIAN ? in_x : Swap64Endian(in_x);
}

/**
 * \brief Converts a value to a little endian value (16).
 * \param in_x Base value
 * \return Converted value 
 */
static RkUint16 ToLittleEndian16(RkUint16 const in_x)
{
    return RUKEN_IS_BIG_ENDIAN ? Swap16Endian(in_x) : in_x;
}

/**
 * \brief Converts a value to a little endian value (32).
 * \param in_x Base value
 * \return Converted value 
 */
static RkUint32 ToLittleEndian32(RkUint32 const in_x)
{
    return RUKEN_IS_BIG_ENDIAN ? Swap32Endian(in_x) : in_x;
}

/**
 * \brief Converts a value to a little endian value (64).
 * \param in_x Base value
 * \return Converted value 
 */
static RkUint64 ToLittleEndian64(RkUint64 const in_x)
{
    return RUKEN_IS_BIG_ENDIAN ? Swap64Endian(in_x) : in_x;
}

END_RUKEN_NAMESPACE