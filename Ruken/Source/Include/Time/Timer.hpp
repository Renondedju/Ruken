
#pragma once

#include "Build/Namespace.hpp"

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

class Timer
{
    private:

        RkVoid* m_win_handle;

    public:

        #pragma region Constructors

        Timer () noexcept;
        Timer (Timer const& in_copy) = delete;
        Timer (Timer&&      in_move) = delete;
        ~Timer() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Sets the timer timing in nanoseconds
         * \param in_nanoseconds Amount of nanoseconds to set
         * \return true if the operation succeeded, false otherwise
         */
        RkBool SetTiming(RkInt64 in_nanoseconds) const noexcept;

        /**
         * \brief Sleeps for x amount of nanoseconds (set via SetTiming())
         * \return true if the operation succeeded, false otherwise
         */
        RkBool NSleep() const noexcept;

        #pragma endregion

        #pragma region Operators

        Timer& operator=(Timer const& in_copy) = delete;
        Timer& operator=(Timer&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE