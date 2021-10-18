/**
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

#include "Build/Namespace.hpp"
#include "Types/Concepts/FundamentalType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Eases the creation of strong types
 * 
 * Every method is constexpr and noexcept if the base type is noexcept as well
 * 
 * To create a "complex" StrongType, simply inherit from this class and overload all the required operators.
 * Do note that every operator class like Addition or Modulo, can also be inherited to save you some time.
 * 
 * Example :
 * 
 * // This class is strongly typed and only has the addition and subtraction operators available
 * struct Meter : StrongType<float, Meter>, Addition<Meter>, Subtraction<Meter>
 * { using StrongType::StrongType };
 * 
 * \tparam TBase Base type, could be any fundamental type
 * \tparam TUniquePhantom Phantom type, this should be unique for each type
 * \see https://foonathan.net/blog/2016/10/19/strong-typedefs.html
 * \see https://arne-mertz.de/2016/11/stronger-types/
 */
template <FundamentalType TBase, typename TUniquePhantom>
class StrongType
{
    #pragma region Members

    TBase m_value {};

    #pragma endregion

    public:

        using UnderlyingType = TBase;

        #pragma region Constructors

        explicit constexpr StrongType(TBase const& in_copy) noexcept;
        explicit constexpr StrongType(TBase&&      in_move) noexcept;

        constexpr StrongType()                          = default;
        constexpr StrongType(StrongType const& in_copy) = default;
        constexpr StrongType(StrongType&&      in_move) = default;
                 ~StrongType()                          = default;

        #pragma endregion

        #pragma region Operators

        template <typename TType> requires std::is_convertible_v<TBase, TType>
        explicit constexpr operator TType() const noexcept
        {
            return static_cast<TType>(m_value);
        }

        constexpr StrongType& operator=(StrongType const& in_copy) = default;
        constexpr StrongType& operator=(StrongType&&      in_move) = default;

        #pragma endregion
};

#include "Types/StrongType/StrongType.inl"

END_RUKEN_NAMESPACE