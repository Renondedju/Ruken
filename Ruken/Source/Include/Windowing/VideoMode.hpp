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

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This describes a single video mode.
 */
struct VideoMode
{
    /**
     * \brief The width, in screen coordinates, of the video mode.
     */
    RkInt32 width {0};

    /**
     * \brief The height, in screen coordinates, of the video mode.
     */
    RkInt32 height {0};

    /**
     * \brief The bit depth of the red channel of the video mode.
     */
    RkInt32 red_bits {0};

    /**
     * \brief The bit depth of the green channel of the video mode.
     */
    RkInt32 green_bits {0};

    /**
     * \brief The bit depth of the blue channel of the video mode.
     */
    RkInt32 blue_bits {0};

    /**
     * \brief The refresh rate, in Hz, of the video mode.
     */
    RkInt32 refresh_rate {0};
};

END_RUKEN_NAMESPACE