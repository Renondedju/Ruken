#pragma once

#include "Core/ExecutiveSystem/CPU/Awaitables/CPUAwaitable.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief
 */
class AutomaticResetEvent final : public CPUAwaitable<RkVoid, true>
{
    protected:

        RkVoid Deallocate() override {}

    public:

        #pragma region Lifetime

        AutomaticResetEvent()                           = default;
        AutomaticResetEvent(AutomaticResetEvent const&) = delete;
        AutomaticResetEvent(AutomaticResetEvent&&)      = delete;
        ~AutomaticResetEvent() noexcept override
        {
            Consume();
        }

        AutomaticResetEvent& operator=(AutomaticResetEvent const&) = delete;
        AutomaticResetEvent& operator=(AutomaticResetEvent&&)      = delete;

        #pragma endregion

        #pragma region Methods

        /// Signals the event
        RkVoid Signal() const noexcept { CPUAwaitable::Signal(); }

        #pragma endregion
};

END_RUKEN_NAMESPACE