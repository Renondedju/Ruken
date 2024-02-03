#pragma once

#include <algorithm>

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TResult>
class CPUAwaitableResultStorage
{
    protected:

        #pragma region Members

        TResult m_result;

        #pragma endregion

    public:

        #pragma region Methods

        /**
         * \brief Returns the result of the awaitable.
         * \warning Do note that this result is valid only if the awaitable has been completed.
         * \return The result value
         */
        [[nodiscard]]
        TResult const& GetResult() const noexcept
        { return m_result; }


		// ----------------------------------------------------
        // The following methods are coroutine promises helpers
        // and can be ignored otherwise. This avoids having
        // to do a template specialization later.

        /**
         * \brief Called when the coroutine returns a value
         * \param in_result Result emitted by the coroutine
         */
        void return_value(TResult&& in_result) noexcept
        {   m_result = std::forward<TResult>(in_result); }

        void return_value(TResult const& in_result) noexcept
            requires !std::is_reference_v<TResult>
        {   m_result = in_result; }

        #pragma endregion
};

template <>
class CPUAwaitableResultStorage<RkVoid>
{
    public:

        // The following method is a coroutine promise helper
        // and can be ignored otherwise. This avoids having
        // to do a template specialization later.

        constexpr void return_void() noexcept { }
};

END_RUKEN_NAMESPACE