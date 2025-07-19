#pragma once

#include "Build/Namespace.hpp"

#include <shared_mutex>

BEGIN_RUKEN_NAMESPACE

// Forward declaration
template <typename TType>
struct SynchronizedAccess;

/**
 * @brief Synchronized class allows for safe multi-threader read/write access
 * @tparam TValue Type of the synchronized value
 * 
 * @note If you plan on using this class with fundamental types, using a std::atomic instead might be a better choice for you (performance wise)
 * 
 * Here are some advantages of using the Synchronized class instead of a pure c++ mutex
 * 
 * Much safer and expressive than pure C++
 *    - Clearly identify the value objects that need to be protected.
 *    - Clearly associate the mutex with the values objects they protect.
 *    - Clearly distinguish protected and unprotected accesses.
 *    - Prevent unwanted unprotected accesses.
 *    
 * Simple and easy to use
 *    - Simply replace your mutex by Synchronized objects and locks by SynchronizedAccess objects.
 * 
 * TODO: Synchronized move and copy operators/constructors
 */
template<typename TValue>
struct Synchronized
{
    // Allows the exclusive access to m_mutex
    using ReadAccess     = SynchronizedAccess<TValue const>;
    using WriteAccess    = SynchronizedAccess<TValue>;
    using UnderlyingType = TValue;

	friend ReadAccess;
	friend WriteAccess;

	#pragma region Lifetime

	/**
	 * @brief Constructs the content of the synchronized object using the following constructor: Type(Args...).
	 * @tparam TArgs Arguments type.
	 * @param in_args arguments.
	 */
	template <typename... TArgs> requires std::is_constructible_v<TValue, TArgs...>
	 explicit Synchronized  (TArgs&&... in_args)		  noexcept(noexcept(TValue(in_args...)));
	 Synchronized           (Synchronized const& in_copy) noexcept;
	 Synchronized           (Synchronized&&      in_move) noexcept;
	 Synchronized& operator=(Synchronized const& in_copy) = delete;
	 Synchronized& operator=(Synchronized&&      in_move) = delete;
	~Synchronized()										  = default;

	#pragma endregion

	/**
	 * @brief Unsafe single time access to content of the synchronized object.
	 * @warning This method should be used wisely. Concurrent access of the same value will cause data races and thus undefined behaviors !
	 * @return Synchronized object's content.
	 */
	[[nodiscard]] TValue const& Unsafe() const noexcept;
	[[nodiscard]] TValue&       Unsafe()       noexcept;

	/// @brief Returns a read access.
	[[nodiscard]] ReadAccess Read() noexcept;

	/// @brief Returns a write access.
	[[nodiscard]] WriteAccess Write() noexcept;

    private:

		TValue                    m_value;
        mutable std::shared_mutex m_mutex {};
};

END_RUKEN_NAMESPACE

#include "Synchronized.inl"
