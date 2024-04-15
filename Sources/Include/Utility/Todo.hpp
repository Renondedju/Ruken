
#pragma once

#include "Build/Namespace.hpp"

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

#if defined(RUKEN_COMPILER_MSVC) && _MSC_VER <= 1800
    #define TODO(...) // constexpr wont work :(
#else

namespace internal
{
    struct Todo
    {
        template<unsigned TN>
        static constexpr RkUint64 Date(RkChar const (&in_dt)[TN] )
        {
            return static_cast<RkUint64>(
                    /*D*/ ( in_dt[4] == ' ' ? 0 : (in_dt[4] - '0') * 10 ) + (in_dt[5] - '0') +
                    /*M*/ ( in_dt[2] == 'n' ? 1
                          : in_dt[2] == 'b' ? 2
                          : in_dt[2] == 'r' && in_dt[0] == 'M' ? 3
                          : in_dt[2] == 'r' && in_dt[0] != 'M' ? 4
                          : in_dt[2] == 'y' ? 5
                          : in_dt[2] == 'n' ? 6
                          : in_dt[2] == 'l' ? 7
                          : in_dt[2] == 'g' ? 8
                          : in_dt[2] == 'p' ? 9
                          : in_dt[2] == 't' ? 10
                          : in_dt[2] == 'v' ? 11 : 12 ) * 100 +
                    /*Y*/ ( in_dt[7] -  '0') * 1e7 + (in_dt[8] - '0') * 1e6 + (in_dt[9] - '0') * 1e5 + (in_dt[10] - '0') * 1e4);
        }

        template<unsigned TN>
        static constexpr bool Check( RkChar const (&in_dt)[TN] )
        {
            return Date(__DATE__) < Date(in_dt);
        }
    };
}

/**
 * \brief This macro will stop any compilation if in_datetime >= to the current date
 * \param in_datetime deadline date
 * \param in_task task to do before the deadline
 * 
 * \note The datetime format should be the same as the __DATE__ macro
 * \example TODO("Sep 26 2015", "Finish the renderer") 
 */
#define TODO(in_datetime, in_task) static_assert(internal::Todo::Check(in_datetime), "TODO date expired: " in_task)

#endif

END_RUKEN_NAMESPACE