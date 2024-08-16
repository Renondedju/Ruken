#pragma once

#include <algorithm>

#include "../../Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename TValue>
class ValueStorage
{
    protected:

        #pragma region Members

        TValue m_value;

        #pragma endregion

    public:

        #pragma region Methods

        template <typename... TArgs>
        explicit ValueStorage(TArgs&&... in_args) noexcept
            requires std::is_constructible_v<TValue, TArgs...>:
            m_value {std::forward<TArgs...>(in_args)...}
        {}

        /**
         * \brief Returns the result of the awaitable.
         * \warning Do note that this result is valid only if the awaitable has been completed.
         * \return The result value
         */
        [[nodiscard]]
        TValue const& GetValue() const noexcept
        { return m_value; }

		// ----------------------------------------------------
        // The following methods are coroutine promises helpers
        // and can be ignored otherwise. This avoids having
        // to do a template specialization later.

        /**
         * \brief Called when the coroutine returns a value
         * \param in_result Result emitted by the coroutine
         */
        void return_value(TValue&& in_result) noexcept
        {   m_value = std::forward<TValue>(in_result); }

        void return_value(TValue const& in_result) noexcept
            requires (!std::is_reference_v<TValue>)
        {   m_value = in_result; }

        #pragma endregion
};

template <>
class ValueStorage<RkVoid>
{
    public:

        // The following method is a coroutine promise helper
        // and can be ignored otherwise. This avoids having
        // to do a template specialization later.

        constexpr void return_void() noexcept { }
};

END_RUKEN_NAMESPACE