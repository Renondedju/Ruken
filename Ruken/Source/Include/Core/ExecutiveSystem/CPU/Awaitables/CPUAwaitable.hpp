#pragma once

#include "Core/ExecutiveSystem/Awaitable.hpp"
#include "Core/ExecutiveSystem/CPU/CentralProcessingUnit.hpp"
#include "Core/ExecutiveSystem/CPU/Awaitables/Tasks/Suspensions/CPUSuspension.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class acts as a base for every CPU awaitable
 */
class CPUAwaitable: public Awaitable<CentralProcessingUnit>
{
    #pragma region Members

    CPUSuspension::Node m_suspension_node {nullptr};

    #pragma endregion

    public:

        using ProcessingUnit = CentralProcessingUnit;
        using Awaitable::Awaitable;

        #pragma region Methods

        /**
         * \brief Signals the completion of the event to every of the attached awaiters.
         * This will consume the event and make it completed until Reset() has been called.
         */
        RkVoid SignalCompletion() noexcept;

        /**
         * \brief Resets the event, making it non completed and ready to hook new awaiters.
         * Do note that any awaiting event will be lost and won't resume ever if reset is called when
         * Completed() returns false. Make sure to signal a completion before resetting.
         */
        RkVoid Reset() noexcept;

        /**
         * \brief Checks if the event has been completed yet
         * \return True if the event has already been completed, false otherwise
         */
        [[nodiscard]]
		RkBool Completed() const noexcept;

        /**
         * \brief Returns the suspension node to attach to in order to wait for the awaitable
         */
        [[nodiscard]]
        CPUSuspension::Node& GetSuspensionNode() noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE