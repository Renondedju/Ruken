
#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"

#include "Meta/Assert.hpp"
#include "Bitwise/SizedBitmask.hpp"
#include "Types/FundamentalTypes.hpp"

// Used for the RUKEN_BITMASK macro
#include "Meta/Meta.hpp"
#include "Meta/MinimumType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Creates an enum that matches every conditions required to be used used as a bitmask
 * \param name Name of the enum to create
 */
#define RUKEN_BITMASK(name, ...) enum class name : MinimumTypeT<RUKEN_GET_ARG_COUNT(##__VA_ARGS__)> { ##__VA_ARGS__ }; \
    static_assert(RUKEN_GET_ARG_COUNT(##__VA_ARGS__) < 64, "Bitmask enums only support a maximum of 64 values")

namespace internal
{
    template <typename TEnumType, typename... TTypes>
    using AreEnumType = std::enable_if_t<std::conjunction_v<std::is_same<TTypes, TEnumType>...>, RkBool>;
}

/**
 * \brief A bitmask can be used to store multiple flags into a single integer.
 * 
 * All enums used should have continuous values starting from 0. Use the macro RUKEN_BITMASK to safely declare a bitmask.
 * Example : enum class EEnum : RkUint8 { First, Second, Third };
 *
 * This enum is safe since all the elements in the enum are continuous and starts from 0.
 * Also the underlying type of the enum is RkUint8 (8 bits) which is enough to contain all the values (3).
 * 
 * \tparam TEnumType Enum to strong type the bitmask with
 * \note Enum classes are supported
 */
template <typename TEnumType>
class __declspec(novtable) Bitmask : public SizedBitmask<1, std::underlying_type_t<TEnumType>>
{
    RUKEN_STATIC_ASSERT(std::is_enum_v<TEnumType>, "Cannot create a Bitmask from a non enum type");

    public:

        using Parent         = SizedBitmask<1, std::underlying_type_t<TEnumType>>;
        using UnderlyingType = std::underlying_type_t<TEnumType>;

        #pragma region Constructors

        template <typename... TData, internal::AreEnumType<TEnumType, TData...> = true>
        constexpr Bitmask(TData... in_data) noexcept;

        constexpr Bitmask(Bitmask const& in_copy) = default;
        constexpr Bitmask(Bitmask&&      in_move) = default;

        #pragma endregion

        ~Bitmask() noexcept = default;

        #pragma region Methods

        /**
         * \brief Checks if the bitmask has all the flags passed as enabled.
         * \tparam TData Types of the flags to check
         * \param in_data Flags to check
         * \return True if the bitmask has all the specified flags enabled
         */
        template <typename... TData, internal::AreEnumType<TEnumType, TData...> = true>
        [[nodiscard]] constexpr RkBool HasAll(TData...       in_data)    const noexcept;
        [[nodiscard]] constexpr RkBool HasAll(Bitmask const& in_bitmask) const noexcept;

        /**
         * \brief Checks if the bitmask has at least one the flags passed as enabled.
         * \tparam TData Types of the flags to check
         * \param in_data Flags to check
         * \return True if the bitmask has at least one of the specified flags enabled
         */
        template <typename... TData, internal::AreEnumType<TEnumType, TData...> = true>
        [[nodiscard]] constexpr RkBool HasOne(TData...       in_data)    const noexcept;
        [[nodiscard]] constexpr RkBool HasOne(Bitmask const& in_bitmask) const noexcept;

        /**
         * \brief Enables the specified flags.
         * \tparam TData Types of the flags to enable
         * \param in_data Flags to enable
         */
        template <typename... TData, internal::AreEnumType<TEnumType, TData...> = true>
        constexpr RkVoid Add(TData...       in_data)    noexcept;
        constexpr RkVoid Add(Bitmask const& in_bitmask) noexcept;

        /**
         * \brief Disables the specified flags.
         * \tparam TData Types of the flags to disable
         * \param in_data Flags to disable
         */
        template <typename... TData, internal::AreEnumType<TEnumType, TData...> = true>
        constexpr RkVoid Remove(TData...       in_data)    noexcept;
        constexpr RkVoid Remove(Bitmask const& in_bitmask) noexcept;

        /**
         * \brief Executes a function pointer on each enabled flag in the bitmask.
         * \tparam TLambdaType Type of the lambda, the signature of the function used must be RkVoid (*in_lambda)(TEnumType in_flag)
         * \param in_lambda Function pointer or lambda (in case of a lambda, this will automatically be inlined by the compiler)
         */
        template <typename TLambdaType>
        constexpr RkVoid Foreach(TLambdaType in_lambda) const noexcept;

        #pragma endregion

        #pragma region Operators

        Bitmask& operator= (Bitmask const& in_copy) = default;
        Bitmask& operator= (Bitmask&&      in_move) = default;

        [[nodiscard]] constexpr Bitmask operator-(TEnumType const& in_bit) const noexcept;
        [[nodiscard]] constexpr Bitmask operator+(TEnumType const& in_bit) const noexcept;

        constexpr Bitmask& operator-=(TEnumType const& in_bit) noexcept;
        constexpr Bitmask& operator+=(TEnumType const& in_bit) noexcept;

        #pragma endregion
};

#include "Bitwise/Bitmask.inl"

END_RUKEN_NAMESPACE
