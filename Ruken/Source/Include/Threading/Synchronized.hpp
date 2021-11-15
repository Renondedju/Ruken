
#pragma once

#include <shared_mutex>

#include "Build/Namespace.hpp"

#include "Threading/EAccessMode.hpp"

BEGIN_RUKEN_NAMESPACE

// Forward declaration
template<class, EAccessMode>
class SynchronizedAccess;

/**
 * \brief Synchronized class allows for safe multi-threader read/write access
 * \tparam TType Type of the synchronized value
 * 
 * \note If you plan on using this class with fundamental types, using a std::atomic instead might be a better choice for you (performance wise)
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
template<typename TType>
class Synchronized
{
    // Allows the exclusive access to m_mutex
    template<class, EAccessMode> friend class SynchronizedAccess;
    template<class, EAccessMode> friend class BaseSynchronizedAccess;

    public:
        
        using ReadAccess     = SynchronizedAccess<TType, EAccessMode::Read  >;
        using WriteAccess    = SynchronizedAccess<TType, EAccessMode::Write >;
        using UnsafeAccess   = SynchronizedAccess<TType, EAccessMode::Unsafe>;
        using UnderlyingType = TType;

    private:

        #pragma region Variables

        mutable std::shared_mutex m_mutex;
        TType                     m_value;

        #pragma endregion

    public:

        #pragma region Constructors

        /**
         * \brief Constructs the content of the synchronized object using the following constructor: Type(Args...)
         * \tparam TArgs Arguments type
         * \param in_args arguments
         */
        template <typename ...TArgs, typename = std::enable_if_t<std::is_constructible_v<TType, TArgs...>>>
        Synchronized(TArgs... in_args);

        Synchronized()                            noexcept;
        Synchronized(Synchronized const& in_copy) noexcept;
        Synchronized(Synchronized&&      in_move) noexcept;
        ~Synchronized() = default;

        #pragma endregion

        /**
         * \brief Unsafe single time access to content of the synchronized object
         * 
         * \warning This method should be used wisely. Concurrent access of the same value will cause data races and thus undefined behaviors !
         * \return Synchronized object's content
         */
        [[nodiscard]]
        TType const& Unsafe() const noexcept;
        [[nodiscard]]
        TType&       Unsafe()       noexcept;

        #pragma region Operators

        Synchronized& operator=(Synchronized const& in_copy) = delete;
        Synchronized& operator=(Synchronized&&      in_move) = delete;

        #pragma endregion
};

#include "Threading/Synchronized.inl"

END_RUKEN_NAMESPACE