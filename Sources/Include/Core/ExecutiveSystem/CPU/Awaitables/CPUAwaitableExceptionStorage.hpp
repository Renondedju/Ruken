#pragma once

#include <exception>

#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

template <RkBool>
class CPUAwaitableExceptionStorage
{
	protected:

		#pragma region Members

		std::exception_ptr m_exception {nullptr};

		#pragma endregion

	public:

		#pragma region Methods

		/**
         * \brief Returns the exception.
         * \return The exception pointer.
         */
		[[nodiscard]]
	    std::exception_ptr GetException() const noexcept
	    { return m_exception; }

		#pragma endregion
};

template <>
class CPUAwaitableExceptionStorage<true>
{};

END_RUKEN_NAMESPACE