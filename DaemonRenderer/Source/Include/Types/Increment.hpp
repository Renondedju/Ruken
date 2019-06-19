/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#pragma once

#include "Config.hpp"

#include "Types/NamedType.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Increment operator class
 * 
 * This class is meant to be used in conjunction with the NamedType class.
 * This allows for better and quicker operator integrations to named types
 * 
 * \tparam TStrongTypedef Base NamedType
 * \see NamedType
 */
template <typename TStrongTypedef>
struct Increment
{
	/**
	 * \brief Pre-Increment operator
	 * \param in_instance Class instance
	 * \return Reference to the new instance
	 */
	friend constexpr TStrongTypedef& operator++(TStrongTypedef& in_instance) noexcept
	{
		using Type = internal::UnderlyingType<TStrongTypedef>;

		++static_cast<Type&>(in_instance);
		return in_instance;
	}

	/**
	 * \brief Post-Increment operator
	 * \param in_instance Class instance
	 * \return Reference to the new instance
	 */
	friend constexpr TStrongTypedef operator++(TStrongTypedef& in_instance, DAEint) noexcept
	{
		using Type = internal::UnderlyingType<TStrongTypedef>;

		TStrongTypedef const temp(in_instance);
		static_cast<Type&>(in_instance)++;
		return temp;
	}
};

END_DAEMON_NAMESPACE