
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