
#pragma once

#include <shared_mutex>

#include "Build/Namespace.hpp"
#include "Threading/EAccessMode.hpp"

BEGIN_RUKEN_NAMESPACE

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
class SynchronizedAccess
{
    private:

        #pragma region Members

        Synchronized<TData>& m_synchronized;

        #pragma endregion

    public:
    
        #pragma region Constructors

        SynchronizedAccess(Synchronized<TData>& in_synchronized) noexcept;

        SynchronizedAccess(SynchronizedAccess const& in_copy) = default;
        SynchronizedAccess(SynchronizedAccess&&      in_move) = default;
        ~SynchronizedAccess()                                 = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Synchronized getters
         * \return Synchronized's object content
         */
        [[nodiscard]] TData&       Get()       noexcept;
        [[nodiscard]] TData const& Get() const noexcept;

        #pragma endregion 

        #pragma region Operators

        SynchronizedAccess& operator=(SynchronizedAccess const& in_copy) = delete;
        SynchronizedAccess& operator=(SynchronizedAccess&&      in_move) = delete;

        [[nodiscard]] TData&       operator* ()       noexcept;
        [[nodiscard]] TData const& operator* () const noexcept;

        [[nodiscard]] TData*       operator->()       noexcept;
        [[nodiscard]] TData const* operator->() const noexcept;

        #pragma endregion
};

/**
 * \brief Allows safe accesses to a Synchronized object using the RAII principle
 * \brief Read access specialization
 * 
 * \tparam TData Synchronized object's type
 */
template<class TData>
class SynchronizedAccess<TData, EAccessMode::Read>
{
    private:

        #pragma region Members

        Synchronized<TData>&                m_synchronized;
        std::shared_lock<std::shared_mutex> m_lock;

        #pragma endregion 

    public:
    
        #pragma region Constructors

        SynchronizedAccess(Synchronized<TData>& in_synchronized) noexcept;

        SynchronizedAccess(SynchronizedAccess const& in_copy)  = delete;
        SynchronizedAccess(SynchronizedAccess&&      in_move)  = default;
        ~SynchronizedAccess()                                  = default;

        #pragma endregion

        #pragma region Methods

        std::shared_lock<std::shared_mutex>& GetLock() noexcept;

         /**
         * \brief Synchronized getter
         * \return Synchronized's object content
         */
        [[nodiscard]]
        TData const& Get() const noexcept;

        #pragma endregion

        #pragma region Operators

        SynchronizedAccess& operator=(SynchronizedAccess const& in_copy) = delete;
        SynchronizedAccess& operator=(SynchronizedAccess&&      in_move) = delete;

        [[nodiscard]] TData const& operator* () const noexcept;
        [[nodiscard]] TData const* operator->() const noexcept;

        #pragma endregion
};

/**
 * \brief Allows safe accesses to a Synchronized object using the RAII principle
 * \brief Write access specialization
 * 
 * \tparam TData Synchronized object's type
 */
template<class TData>
class SynchronizedAccess<TData, EAccessMode::Write>
{
    private:

        #pragma region Members

        Synchronized<TData>&                m_synchronized;
        std::unique_lock<std::shared_mutex> m_lock;

        #pragma endregion 

    public:
    
        #pragma region Constructors

        SynchronizedAccess(Synchronized<TData>& in_synchronized) noexcept;

        SynchronizedAccess(SynchronizedAccess const& in_copy) = delete;
        SynchronizedAccess(SynchronizedAccess&&      in_move) = default;
        ~SynchronizedAccess()                                 = default;

        #pragma endregion

        #pragma region Methods

        std::unique_lock<std::shared_mutex>& GetLock() noexcept;

        /**
         * \brief Synchronized getters
         * \return Synchronized's object content
         */
        [[nodiscard]] TData&       Get()       noexcept;
        [[nodiscard]] TData const& Get() const noexcept;

        #pragma endregion

        #pragma region Operators

        SynchronizedAccess& operator=(SynchronizedAccess const& in_copy) = delete;
        SynchronizedAccess& operator=(SynchronizedAccess&&      in_move) = delete;

        [[nodiscard]] TData&       operator* ()       noexcept;
        [[nodiscard]] TData const& operator* () const noexcept;

        [[nodiscard]] TData*       operator->()       noexcept;
        [[nodiscard]] TData const* operator->() const noexcept;

        #pragma endregion
};

#include "Threading/SynchronizedAccess.inl"

END_RUKEN_NAMESPACE