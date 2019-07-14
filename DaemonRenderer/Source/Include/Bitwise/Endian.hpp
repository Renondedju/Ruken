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
#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

#define DAEMON_IS_BIG_ENDIAN    (*(DAEuint16 *)"\0\1" == 1)
#define DAEMON_IS_LITTLE_ENDIAN !DAEMON_IS_BIG_ENDIAN

/**
 * \brief Swaps the endianness of the value (16)
 * \param in_x value
 * \return converted value
 */
static DAEuint16 Swap16Endian(DAEuint16 const in_x)
{
	return (in_x << 8) | (in_x >> 8);
}

/**
 * \brief Swaps the endianness of the value (32)
 * \param in_x value
 * \return converted value
 */
static DAEuint32 Swap32Endian(DAEuint32 const in_x)
{
	return (in_x << 24) | (in_x >> 24) |
		((in_x & 0xff00) << 8) | ((in_x >> 8) & 0xff00);
}

/**
 * \brief Swaps the endianness of the value (64)
 * \param in_x value
 * \return converted value
 */
static DAEuint64 Swap64Endian( DAEuint64 const in_x )
{
	return (in_x << 56) | (in_x >> 56) |
		((in_x & 0xff00) << 40) | ((in_x >> 40) & 0xff00) |
		((in_x & 0xff0000) << 24) | ((in_x >> 24) & 0xff0000) |
		((in_x & 0xff000000) << 8) | ((in_x >>  8) & 0xff000000);
}

/**
 * \brief Converts a value to a big endian value (16)
 * \param in_x value
 * \return converted value 
 */
static DAEuint16 ToBigEndian16(DAEuint16 in_x)
{
	return DAEMON_IS_BIG_ENDIAN ? in_x : Swap16Endian(in_x);
}

/**
 * \brief Converts a value to a big endian value (32)
 * \param in_x value
 * \return converted value 
 */
static DAEuint32 ToBigEndian32(DAEuint32 in_x)
{
	return DAEMON_IS_BIG_ENDIAN ? in_x : Swap32Endian(in_x);
}

/**
 * \brief Converts a value to a big endian value (64)
 * \param in_x value
 * \return converted value 
 */
static DAEuint64 ToBigEndian64(DAEuint64 in_x)
{
	return DAEMON_IS_BIG_ENDIAN ? in_x : Swap64Endian(in_x);
}

/**
 * \brief Converts a value to a little endian value (16)
 * \param in_x value
 * \return converted value 
 */
static DAEuint16 ToLittleEndian16(DAEuint16 in_x)
{
	return DAEMON_IS_BIG_ENDIAN ? Swap16Endian(in_x) : in_x;
}

/**
 * \brief Converts a value to a little endian value (32)
 * \param in_x value
 * \return converted value 
 */
static DAEuint32 ToLittleEndian32(DAEuint32 in_x)
{
	return DAEMON_IS_BIG_ENDIAN ? Swap32Endian(in_x) : in_x;
}

/**
 * \brief Converts a value to a little endian value (64)
 * \param in_x value
 * \return converted value 
 */
static DAEuint64 ToLittleEndian64(DAEuint64 in_x)
{
	return DAEMON_IS_BIG_ENDIAN ? Swap64Endian(in_x) : in_x;
}

END_DAEMON_NAMESPACE