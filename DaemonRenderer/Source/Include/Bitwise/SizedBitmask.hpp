/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
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

#include <type_traits>

#include "Config.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

namespace internal
{
    // Used by the SizedBitmask to check if all passed types are integral types
    template <typename... TTypes>
    using CheckIntegralTypes = std::enable_if_t<std::conjunction_v<std::is_integral<TTypes>...>, DAEbool>;
}

/**
 * \brief A bitmask can be used to store multiple flags into a single integer.
 * \tparam TChunk Chunk data type. Must be an integral type.
 * \tparam TSize Number of flags to hold into the bitmask, where flags are packed by chunks.
 *               The number of flags in the bitmask is given by TSize * 8 * sizeof(TChunk)
 *               Using a power of 2 is highly recommended to preserve performances !
 *
 * \note Any operations will cost O(TSize), thus bigger chunk sizes allows better performances
 *       but might be a waste of memory if you don't use every provided flag.
 *       If you use less than 64 flags, then TSize should stay at one and you should decrease
 *       the chunk size accordingly to match your needs.
 */
template <DAEsize TSize, typename TChunk = DAEsize>
class SizedBitmask
{
    static_assert(std::is_integral_v<TChunk>, "TChunk must be an integral type");

    private:

        #pragma region Members

        TChunk m_data[TSize];

        #pragma endregion

    public:

        static constexpr DAEsize sizeof_chunk = sizeof(TChunk) * 8; 
        static constexpr DAEsize flags_count  = TSize * sizeof_chunk;

        #pragma region Constructors

        template <typename... TData, internal::CheckIntegralTypes<TData...> = true>
        constexpr SizedBitmask(TData... in_data) noexcept;

        constexpr SizedBitmask(SizedBitmask const& in_copy) = default;
        constexpr SizedBitmask(SizedBitmask&&      in_move) = default;
                 ~SizedBitmask()                            = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Checks if the bitmask has all the flags passed as enabled.
         * \tparam TData Flags to check must be an integral type
         * \param in_data Flags to check 
         * \return True if the bitmask has all the specified flags enabled
         *
         * \warning If one of the passed flag is bigger than the max amount of flags stored in the bitmask,
         *          this function will result in an undefined behavior, you must make sure that your values are correct !
         */
        template <typename... TData, internal::CheckIntegralTypes<TData...> = true>
        [[nodiscard]] constexpr DAEbool HasAll(TData... in_data)               const noexcept;
        [[nodiscard]] constexpr DAEbool HasAll(SizedBitmask const& in_bitmask) const noexcept;

        /**
         * \brief Checks if the bitmask has at least one the flags passed as enabled.
         * \tparam TData Flags to check must be an integral type
         * \param in_data Flags to check 
         * \return True if the bitmask has at least one of the specified flags enabled
         *
         * \warning If one of the passed flag is bigger than the max amount of flags stored in the bitmask,
         *          this function will result in an undefined behavior, you must make sure that your values are correct !
         */
        template <typename... TData, internal::CheckIntegralTypes<TData...> = true>
        [[nodiscard]] constexpr DAEbool HasOne(TData... in_data)               const noexcept;
        [[nodiscard]] constexpr DAEbool HasOne(SizedBitmask const& in_bitmask) const noexcept;

        /**
         * \brief Returns the number of enabled flags in the bitmask.
         *        The implementation is based on the Brian Kernighan's Algorithm.
         *
         * \return Number of enabled flags
         * \note Time Complexity: O(log n).
         */
        [[nodiscard]] constexpr DAEuint16 Popcnt() const noexcept;

        /**
         * \brief Enables the specified flags.
         * \tparam TData Flags to enable must be an integral type
         * \param in_data Flags to enable
         *
         * \warning If one of the passed flag is bigger than the max amount of flags stored in the bitmask,
         *          this function will result in an undefined behavior, you must make sure that your values are correct !
         */
        template <typename... TData, internal::CheckIntegralTypes<TData...> = true>
        constexpr DAEvoid Add(TData... in_data)               noexcept;
        constexpr DAEvoid Add(SizedBitmask const& in_bitmask) noexcept;

        /**
         * \brief Disables the specified flags.
         * \tparam TData Flags to disable must be an integral type
         * \param in_data Flags to disable
         *
         * \warning If one of the passed flag is bigger than the max amount of flags stored in the bitmask,
         *          this function will result in an undefined behavior, you must make sure that your values are correct !
         */
        template <typename... TData, internal::CheckIntegralTypes<TData...> = true>
        constexpr DAEvoid Remove(TData... in_data)               noexcept;
        constexpr DAEvoid Remove(SizedBitmask const& in_bitmask) noexcept;

        /**
         * \brief Clears the bitmask.
         */
        constexpr DAEvoid Clear() noexcept;

        /**
         * \brief Creates a hash code for the given bitmask
         * \return Generated hash code
         */
        constexpr DAEsize HashCode() const noexcept;

        /**
         * \brief Executes a function pointer on each enabled flag in the bitmask.
         * \tparam TLambdaType Type of the lambda, the signature of the function used must be DAEvoid (*in_lambda)(TEnumType in_flag)
         * \tparam TPreCast Type to cast the value into before sending it into the predicate
         * \param in_lambda Function pointer or lambda (in case of a lambda, this will automatically be inlined by the compiler)
         */
        template <typename TLambdaType, typename TPreCast = TChunk>
        constexpr DAEvoid Foreach(TLambdaType in_lambda) const noexcept;

        #pragma endregion 

        #pragma region Operators

        SizedBitmask& operator= (SizedBitmask const& in_copy) noexcept = default;
        SizedBitmask& operator= (SizedBitmask&&      in_move) noexcept = default;

        constexpr SizedBitmask  operator+ (SizedBitmask const& in_bitmask) const noexcept;
        constexpr SizedBitmask  operator- (SizedBitmask const& in_bitmask) const noexcept;
        constexpr DAEbool       operator==(SizedBitmask const& in_other)   const noexcept;
        constexpr SizedBitmask& operator+=(SizedBitmask const& in_bitmask)       noexcept;
        constexpr SizedBitmask& operator-=(SizedBitmask const& in_bitmask)       noexcept;

        #pragma endregion
};

#include "Bitwise/SizedBitmask.inl"

END_DAEMON_NAMESPACE