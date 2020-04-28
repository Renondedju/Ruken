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

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

struct Position2D
{
    DAEint32 x;
    DAEint32 y;
};

struct Extent2D
{
    DAEint32 width;
    DAEint32 height;
};

struct Scale2D
{
    DAEfloat x;
    DAEfloat y;
};

struct Rect2D
{
    Position2D  position;
    Extent2D    extent;
};

namespace windowing
{
    DAEchar const* ToString(DAEint32 in_error_code) noexcept;
}

END_DAEMON_NAMESPACE