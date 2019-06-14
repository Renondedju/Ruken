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

#include <shared_mutex>

#include "Config.hpp"
#include "Threading/EAccessMode.hpp"
#include "Threading/BaseSynchronizedAccess.hpp"

BEGIN_DAEMON_NAMESPACE

template <typename TType>
class Synchronized;

/**
 * \brief Allows safe accesses to a Synchronized object using the RAII principle
 * 
 * This specialization allows unsafe accesses. This is the default specialization. 
 * 
 * \tparam TData Synchronized object's type
 * \tparam TMode Access mode 
 */
template<class TData, EAccessMode TMode>
class SynchronizedAccess final : public BaseSynchronizedAccess<TData, true>
{
	public:
	
		#pragma region Constructors

		SynchronizedAccess(Synchronized<TData>& in_synchronized) noexcept;
		SynchronizedAccess()									 noexcept = delete;
		SynchronizedAccess(SynchronizedAccess const& in_copy)	 noexcept = default;
		SynchronizedAccess(SynchronizedAccess&&		 in_move)	 noexcept = default;
		~SynchronizedAccess()									 noexcept = default;

		#pragma endregion

		#pragma region Operators

		SynchronizedAccess& operator=(SynchronizedAccess const& in_copy) noexcept = default;
		SynchronizedAccess& operator=(SynchronizedAccess&&		in_move) noexcept = default;

		#pragma endregion
};

/**
 * \brief Allows safe accesses to a Synchronized object using the RAII principle
 * \brief Read access specialization
 * 
 * \tparam TData Synchronized object's type
 */
template<class TData>
class SynchronizedAccess<TData, EAccessMode::Read> final : public BaseSynchronizedAccess<TData, false>
{
	private:

		#pragma region Variables

        std::shared_lock<std::shared_mutex> m_lock;

		#pragma endregion 

	public:
	
		#pragma region Constructors

		SynchronizedAccess(Synchronized<TData>& in_synchronized) noexcept;
		SynchronizedAccess()									 noexcept = delete;
		SynchronizedAccess(SynchronizedAccess const& in_copy)	 noexcept = default;
		SynchronizedAccess(SynchronizedAccess&&		 in_move)	 noexcept = default;
		~SynchronizedAccess()									 noexcept = default;

		#pragma endregion

		#pragma region Methods

		std::shared_lock<std::shared_mutex>& GetLock() noexcept;

		#pragma endregion

		#pragma region Operators

		SynchronizedAccess& operator=(SynchronizedAccess const& in_copy) noexcept = default;
		SynchronizedAccess& operator=(SynchronizedAccess&&		in_move) noexcept = default;

		#pragma endregion
};

/**
 * \brief Allows safe accesses to a Synchronized object using the RAII principle
 * \brief Write access specialization
 * 
 * \tparam TData Synchronized object's type
 */
template<class TData>
class SynchronizedAccess<TData, EAccessMode::Write> final : public BaseSynchronizedAccess<TData, true>
{
	private:

		#pragma region Variables

        std::unique_lock<std::shared_mutex> m_lock;

		#pragma endregion 

	public:
	
		#pragma region Constructors

		SynchronizedAccess(Synchronized<TData>& in_synchronized) noexcept;
		SynchronizedAccess()									 noexcept = delete;
		SynchronizedAccess(SynchronizedAccess const& in_copy)	 noexcept = default;
		SynchronizedAccess(SynchronizedAccess&&		 in_move)	 noexcept = default;
		~SynchronizedAccess()									 noexcept = default;

		#pragma endregion

		#pragma region Methods

		std::unique_lock<std::shared_mutex>& GetLock() noexcept;

		#pragma endregion

		#pragma region Operators

		SynchronizedAccess& operator=(SynchronizedAccess const& in_copy) noexcept = default;
		SynchronizedAccess& operator=(SynchronizedAccess&&		in_move) noexcept = default;

		#pragma endregion
};

#include "Threading/SynchronizedAccess.inl"

END_DAEMON_NAMESPACE