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

#include <ostream>
#include <type_traits>

#include "Config.hpp"
#include "Types/FundamentalTypes.hpp"

#include "Meta/Meta.hpp"
#include "Meta/MinimumType.hpp"

BEGIN_DAEMON_NAMESPACE

#define DAEMON_BITMASK(name, ...) enum class name : MinimumTypeT<DAEMON_GET_ARG_COUNT(##__VA_ARGS__)> { ##__VA_ARGS__ }; \
    static_assert(DAEMON_GET_ARG_COUNT(##__VA_ARGS__) < 64, "Bitmask enums only supports a max of 64 values")

/**
 * \brief A bitmask can be used to store multiple flags into a single integer.
 * \warning All enums used should have continuous values starting from 0. Use the macro DAEMON_BITMASK to safely declare a bitmask
 * 
 * \example
 * enum class EEnum : DAEuint8 { First, Second, Third };
 * // This enum is safe since all the elements in the enum are continuous and starts from 0
 * // Also the underlying type of the enum is DAEuint8 (8 bits) which is enough to contain all the values (3)
 * 
 * \tparam TEnum_Type Enum to strong type the bitmask with
 * \note Enum classes are supported
 */
template<typename TEnum_Type>
class Bitmask
{
    static_assert(std::is_enum<TEnum_Type>::value, "Cannot create a Bitmask from a non enum type");

    public: using Underlying_Type = typename std::underlying_type<TEnum_Type>::type;

    private:

        #pragma region Variables

        Underlying_Type m_data;

        #pragma endregion

        #pragma region Constructors

        constexpr Bitmask(Underlying_Type in_data) noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        template <typename... TData, typename = std::enable_if_t<std::conjunction_v<std::is_same<TData, TEnum_Type>...>>>
        constexpr Bitmask(TData... in_data)    noexcept;
        constexpr Bitmask()                    noexcept;

        constexpr Bitmask(Bitmask const& in_copy) noexcept = default;
        constexpr Bitmask(Bitmask&&         in_move) noexcept = default;
        
        ~Bitmask() noexcept = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Checks if the bitmask has all the flags passed as enabled
         * \tparam TData Types of the data to pass
         * \param in_data Data
         * \return True if the bitmask has the targeted flags enabled
         */
        template <typename... TData, typename = std::enable_if_t<std::conjunction_v<std::is_same<TData, TEnum_Type>...>>>
        [[nodiscard]]
        constexpr DAEbool HasAll(TData... in_data)            const noexcept;
        [[nodiscard]]
        constexpr DAEbool HasAll(Bitmask const& in_bitmask) const noexcept;

        /**
         * \brief Checks if the bitmask has at least one the flags passed as enabled
         * \tparam TData Types of the data to pass
         * \param in_data Data
         * \return True if the bitmask has the targeted flags enabled
         */
        template <typename... TData, typename = std::enable_if_t<std::conjunction_v<std::is_same<TData, TEnum_Type>...>>>
        [[nodiscard]]
        constexpr DAEbool HasOne(TData... in_data)            const noexcept;
        [[nodiscard]]
        constexpr DAEbool HasOne(Bitmask const& in_bitmask) const noexcept;

        /**
         * \brief Enables flags
         * \tparam TData Types of the data to pass
         * \param in_data Data
         */
        template <typename... TData, typename = std::enable_if_t<std::conjunction_v<std::is_same<TData, TEnum_Type>...>>>
        constexpr DAEvoid Add(TData... in_data)                noexcept;
        constexpr DAEvoid Add(Bitmask const& in_bitmask)    noexcept;

        /**
         * \brief Disable flags
         * \tparam TData Types of the data to pass
         * \param in_data Data
         */
        template <typename... TData, typename = std::enable_if_t<std::conjunction_v<std::is_same<TData, TEnum_Type>...>>>
        constexpr DAEvoid Remove(TData... in_data)            noexcept;
        constexpr DAEvoid Remove(Bitmask const& in_bitmask)    noexcept;

        /**
         * \brief Clears the bitmask
         */
        constexpr DAEvoid Clear() noexcept;

        /**
         * \brief Returns the number of enabled flags in the bitmask
         * 
         * Time Complexity: O(log n)
         * The implementation is based on the Brian Kernighan's Algorithm.
         *
         * I know that __popcnt16, __popcnt, __popcnt64 is a thing but I don't want to be dependent
         * of the ABM instruction set since it has been dropped on recent modern AMD CPUs
         * and isn't supported by many other processors such as ARM CPUs.
         *
         * \return count of enabled flags
         */
        [[nodiscard]]
        constexpr DAEuint16 Enabled() const noexcept;

        /**
         * \brief Executes a function pointer for each flag enabled in the set
         * \tparam TLambda_Type Type of the lambda, the signature of the function used must be PBRPBRvoid (*in_lambda)(Enum_Type in_flag);
         * \param in_lambda Function pointer or lambda (in case of a lambda, this will automatically be inlined by the compiler)
         */
        template <typename TLambda_Type>
        constexpr DAEvoid Foreach(TLambda_Type in_lambda) const noexcept;

        /**
         * \brief Returns the data stored by the bitmask
         * \return Data stored by the bitmask
         */
        [[nodiscard]]
        constexpr Underlying_Type  Data() const noexcept;
        [[nodiscard]]
        constexpr Underlying_Type& Data()        noexcept;

        #pragma endregion

        #pragma region Operators

        Bitmask& operator= (Bitmask const&  in_copy)                        noexcept = default;
        Bitmask& operator= (Bitmask&&        in_move)                         noexcept = default;

        constexpr Bitmask  operator+ (TEnum_Type const& in_bit)        const    noexcept;
        constexpr Bitmask  operator+ (Bitmask   const& in_bitmask)    const    noexcept;

        constexpr Bitmask  operator+=(TEnum_Type const& in_bit)                noexcept;
        constexpr Bitmask& operator+=(Bitmask   const& in_bitmask)            noexcept;

        constexpr Bitmask  operator- (TEnum_Type const& in_bit)        const    noexcept;
        constexpr Bitmask  operator- (Bitmask   const& in_bitmask)    const    noexcept;

        constexpr Bitmask& operator-=(TEnum_Type const& in_bit)                noexcept;
        constexpr Bitmask& operator-=(Bitmask   const& in_bitmask)            noexcept;

        constexpr DAEbool  operator==(Bitmask   const& in_bitmask)    const    noexcept;

        #pragma endregion
};

#pragma region External Operators

template<typename TEnum_Type>
std::ostream& operator<<(std::ostream& out_stream, Bitmask<TEnum_Type> const& in_bitmask);

#pragma endregion

#include "Bitwise/Bitmask.inl"

END_DAEMON_NAMESPACE
