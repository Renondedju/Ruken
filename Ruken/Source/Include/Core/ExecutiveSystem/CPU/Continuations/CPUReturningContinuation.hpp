#pragma once

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/CPU/Continuations/CPUContinuation.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TReturnType = RkVoid>
class CPUReturningContinuation: public CPUContinuation
{
    #pragma region Members

    CPUAwaitableHandle<TReturnType> m_handle {nullptr};

    #pragma endregion   

    public:

        #pragma region Methods

        /**
         * \brief Sets the hook for this continuation
         */
        RkVoid Setup(CPUAwaiter& in_owner, CPUAwaitableHandle<TReturnType> const& in_awaited) noexcept
        {
            m_handle = in_awaited;

            CPUContinuation::Setup(in_owner, m_handle);
        }
    
        /**
         * \brief Returns a reference to the result of the awaitable
         * \return Reference to the return value
         */
        [[nodiscard]]
        TReturnType const& GetReturnValue() const noexcept
        {
            return m_handle.GetResult();
        }

        #pragma endregion
};

template <>
class CPUReturningContinuation<RkVoid>: public CPUContinuation
{};

END_RUKEN_NAMESPACE