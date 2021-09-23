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

#ifndef RUKEN_INTERNAL_VECTOR_INCLUDE_PASS
    #error Vector2.hpp is meant to be included from the Maths/Vector/Vector.hpp file
#endif

#include "Maths/Vector/BaseVector.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Types/Concepts/ArithmeticType.hpp"

BEGIN_RUKEN_NAMESPACE

template <ArithmeticType TType> struct Vector3;
template <ArithmeticType TType> struct Vector4;

/**
 * \brief Vector 2 class 
 * \tparam TType Underlying type, defaults to RkFloat
 */
template <ArithmeticType TType = RkFloat>
struct __declspec(novtable) Vector2 final: public BaseVector<Vector2<TType>, 2UI64, TType>
{
    #pragma region Members

    #pragma warning(push)
    #pragma warning(disable : 4201) // Warning C4201 nonstandard extension used: nameless struct/union

    union
    {
        TType data[2UI64] {
            static_cast<TType>(0),
            static_cast<TType>(0)
        };

        struct
        {
            TType x;
            TType y;
        };
    };

    #pragma warning(pop) 

    #pragma endregion

    #pragma region Constructors

    using BaseVector<Vector2<TType>, 2UI64, TType>::BaseVector;

    /**
     * \brief Component constructor
     * \param in_x X component
     * \param in_y Y component
     */
    constexpr Vector2(TType in_x, TType in_y) noexcept;

    constexpr Vector2()                       = default;
    constexpr Vector2(Vector2 const& in_copy) = default;
    constexpr Vector2(Vector2&&      in_move) = default;
             ~Vector2()                       = default;

    constexpr Vector2(BaseVector<Vector2<TType>, 2UI64, TType> const& in_copy) noexcept;
    constexpr Vector2(BaseVector<Vector2<TType>, 2UI64, TType>&&      in_move) noexcept;

    #pragma endregion

    #pragma region Operators

    constexpr Vector2& operator=(Vector2 const& in_copy) noexcept = default;
    constexpr Vector2& operator=(Vector2&&	    in_move) noexcept = default;

    constexpr Vector2& operator=(BaseVector<Vector2<TType>, 2UI64, TType> const& in_copy) noexcept;
    constexpr Vector2& operator=(BaseVector<Vector2<TType>, 2UI64, TType>&&      in_move) noexcept;
        
    explicit constexpr operator Vector3<TType>() const noexcept;
    explicit constexpr operator Vector4<TType>() const noexcept;

    #pragma endregion
};

#include "Maths/Vector/Vector2.inl"

using Vector2i = Vector2<RkInt>;
using Vector2f = Vector2<RkFloat>;

// Pre instantiation
extern template struct Vector2<RkInt>;
extern template struct Vector2<RkFloat>;

END_RUKEN_NAMESPACE