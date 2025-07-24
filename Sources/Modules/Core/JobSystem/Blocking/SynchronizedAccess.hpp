#pragma once

#include "Build/Namespace.hpp"

#include <type_traits>

BEGIN_RUKEN_NAMESPACE

template <typename TValue>
struct Synchronized;

/**
 * @brief Allows safe accesses to a Synchronized object using the RAII principle
 * @tparam TValue Synchronized object's type. Can be const for a readonly value.
 */
template<typename TValue>
struct SynchronizedAccess
{
    #pragma region Lifetime

    /**
     * Default constructor.
     * @param in_value Protected value.
     */
    explicit SynchronizedAccess(Synchronized<std::remove_cv_t<TValue>>& in_value) noexcept;
    SynchronizedAccess           (SynchronizedAccess const&) = default;
    SynchronizedAccess           (SynchronizedAccess&&     ) = default;
    SynchronizedAccess& operator=(SynchronizedAccess const&) = delete;
    SynchronizedAccess& operator=(SynchronizedAccess&&     ) = delete;
    ~SynchronizedAccess() noexcept;

    #pragma endregion

    #pragma region Methods

    /// @brief Accessors.
	[[nodiscard]] TValue& Get       () const noexcept;
    [[nodiscard]] TValue& operator* () const noexcept;
    [[nodiscard]] TValue* operator->() const noexcept;

    #pragma endregion

	private:

		Synchronized<std::remove_cv_t<TValue>>& m_value;
};

END_RUKEN_NAMESPACE

#include "SynchronizedAccess.inl"
