#pragma once

#include "Types/FundamentalTypes.hpp"
#include "Core/ExecutiveSystem/CPU/Continuations/CPUContinuation.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TResult = RkVoid, RkBool TNoexcept = true>
class CPUPropagatingContinuation: public CPUContinuation
{
    #pragma region Members

    CPUAwaitableHandle<TResult, TNoexcept> m_handle {nullptr};

    #pragma endregion   

    public:

        #pragma region Methods

        /**
         * \brief Sets the hook for this continuation
         */
        RkVoid Setup(CPUAwaiter& in_owner, CPUAwaitableHandle<TResult, TNoexcept> const& in_awaited) noexcept
        {
            m_handle = in_awaited;

            CPUContinuation::Setup(in_owner, m_handle);
        }
    
        /**
         * \brief Returns a reference to the result of the awaitable.
         * \return Reference to the return value
         */
        [[nodiscard]] auto GetReturnValue() const noexcept
			requires !std::is_same_v<TResult, RkVoid>
        { return m_handle.GetResult(); }

        /**
         * \brief Returns the raised exception pointer if any.
         * \note If an exception is set, reading the result of the continuation is undefined behavior.
         * \return Exception pointer
         */
        [[nodiscard]] std::exception_ptr GetException() const noexcept
			requires !TNoexcept
        { return m_handle.GetException(); }

        #pragma endregion
};

template <>
class CPUPropagatingContinuation<RkVoid, true>: public CPUContinuation
{};

END_RUKEN_NAMESPACE