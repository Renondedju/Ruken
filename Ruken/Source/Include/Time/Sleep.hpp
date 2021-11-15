
#pragma once

#include "Build/Namespace.hpp"

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

struct Sleep
{
    /**
     * \brief Native nanosecond sleep
     * \param in_nanoseconds nanoseconds to sleep for
     * \return true if the operation succeeded, false otherwise
     */
    static RkBool NsSleep(RkInt64 in_nanoseconds) noexcept;

    /**
     * \brief Guaranteed to work on every platform but might use more CPU and be less precise than a native sleep
     * \brief ~ 0.5 ms precision, 1 ms min sleep
     * \param in_seconds Seconds to sleep
     */
    static RkVoid StdSleep(RkDouble in_seconds) noexcept;
};

END_RUKEN_NAMESPACE