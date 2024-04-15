
#pragma once

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This describes the gamma ramp for a monitor.
 */
struct GammaRamp
{
    /**
     * \brief An array of value describing the response of the red channel.
     */
    RkUint16* red {nullptr};

    /**
     * \brief An array of value describing the response of the green channel.
     */
    RkUint16* green {nullptr};

    /**
     * \brief An array of value describing the response of the blue channel.
     */
    RkUint16* blue {nullptr};

    /**
     * \brief The number of elements in each array.
     */
    RkUint32 size {0u};
};

END_RUKEN_NAMESPACE