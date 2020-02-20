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

#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Types/FundamentalTypes.hpp"

#include "Containers/String.hpp"
#include "Containers/Vector.hpp"

BEGIN_DAEMON_NAMESPACE

struct Position2D
{
    DAEint32 x = 0;
    DAEint32 y = 0;
};

struct Extent2D
{
    DAEint32 width  = 0;
    DAEint32 height = 0;
};

struct Scale2D
{
    DAEfloat x = 0.0f;
    DAEfloat y = 0.0f;
};

struct Area2D
{
    Position2D  position;
    Extent2D    extent;
};

struct VideoMode
{
    DAEint32 width;
    DAEint32 height;
    DAEint32 red_bits;
    DAEint32 green_bits;
    DAEint32 blue_bits;
    DAEint32 refresh_rate;
};

struct GammaRamp
{
    DAEuint16*  red;
    DAEuint16*  green;
    DAEuint16*  blue;
    DAEuint32   size;
};

struct WindowParameters
{
    String      name;
    Position2D  position;
    Extent2D    size;
    DAEfloat    opacity;
    DAEbool     fullscreen;
    DAEbool     focused;
    DAEbool     iconified;
    DAEbool     maximized;
    DAEbool     visible;
    DAEbool     resizable;
    DAEbool     decorated;
    DAEbool     auto_iconified;
    DAEbool     floating;
    DAEbool     transparent_framebuffer;
    DAEbool     focused_on_show;
};

END_DAEMON_NAMESPACE