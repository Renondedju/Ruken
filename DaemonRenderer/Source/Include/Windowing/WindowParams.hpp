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

#include "Windowing/Metrics.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This struct contains the parameters used when creating a new window.
 * \note  It can also be retrieved from an existing one.
 */
struct WindowParams
{
    /**
     * \brief The UTF-8 encoded window title.
     */
    DAEchar const* name {"NONE"};

    /**
     * \brief The position, in screen coordinates, of the upper-left corner of the content area of the window.
     */
    Position2D position {};

    /**
     * \brief The size, in screen coordinates, of the content area of the window.
     */
    Extent2D size {};

    /**
     * \brief The opacity (or alpha) value is a positive finite number between 0 and 1.
     * \note  0 is fully transparent and 1 is fully opaque.
     */
    DAEfloat opacity {1.0f};

    DAEbool fullscreen              {false};
    DAEbool resizable               {true};
    DAEbool visible                 {true};
    DAEbool decorated               {true};
    DAEbool focused                 {true};
    DAEbool auto_iconified          {true};
    DAEbool floating                {false};
    DAEbool maximized               {false};
    DAEbool cursor_centered         {true};
    DAEbool transparent_framebuffer {false};
    DAEbool focused_on_show         {true};
    DAEbool scale_to_monitor        {false};
};

END_RUKEN_NAMESPACE