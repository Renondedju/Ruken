
#pragma once

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

enum class ELogLevel : RkUint8
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

END_RUKEN_NAMESPACE