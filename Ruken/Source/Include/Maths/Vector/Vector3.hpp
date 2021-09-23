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
    #error Vector3.hpp is meant to be included from the Maths/Vector/Vector.hpp file
#endif

#include "Maths/Vector/BaseVector.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Types/Concepts/ArithmeticType.hpp"

BEGIN_RUKEN_NAMESPACE

template <ArithmeticType TType> struct Vector2;
template <ArithmeticType TType> struct Vector4;

/**
 * \brief Vector 3 class 
 * \tparam TType Underlying type, defaults to RkFloat
 */
template <ArithmeticType TType = RkFloat>
struct __declspec(novtable) Vector3 final: public BaseVector<Vector3<TType>, 3UI64, TType>
{
    #pragma region Members

    #pragma warning(push)
    #pragma warning(disable : 4201) // Warning C4201 nonstandard extension used: nameless struct/union

    union
    {
        TType data[3UI64] {
            static_cast<TType>(0),
            static_cast<TType>(0),
            static_cast<TType>(0)
        };

        struct
        {
            TType x;
            TType y;
            TType z;
        };
    };

    #pragma warning(pop) 

    #pragma endregion

    #pragma region Constructors

    using BaseVector<Vector3<TType>, 3UI64, TType>::BaseVector;

    /**
     * \brief Component constructor
     * \param in_x X component
     * \param in_y Y component
     * \param in_z Z component
     */
    constexpr Vector3(TType in_x, TType in_y, TType in_z) noexcept;

    constexpr Vector3()                         = default;
    constexpr Vector3(Vector3 const& in_vector) = default;
    constexpr Vector3(Vector3&&      in_vector) = default;
             ~Vector3()                         = default;

    constexpr Vector3(BaseVector<Vector3<TType>, 3UI64, TType> const& in_copy) noexcept;
    constexpr Vector3(BaseVector<Vector3<TType>, 3UI64, TType>&&      in_move) noexcept;

    #pragma endregion

    #pragma region Operators

    constexpr Vector3& operator=(Vector3 const& in_other) noexcept = default;
    constexpr Vector3& operator=(Vector3&&	    in_other) noexcept = default;

    constexpr Vector3& operator=(BaseVector<Vector3<TType>, 3UI64, TType> const& in_copy) noexcept;
    constexpr Vector3& operator=(BaseVector<Vector3<TType>, 3UI64, TType>&&      in_move) noexcept;

    explicit constexpr operator Vector2<TType>() const noexcept;
    explicit constexpr operator Vector4<TType>() const noexcept;

    #pragma endregion
};

#include "Maths/Vector/Vector3.inl"

using Vector3i = Vector3<RkInt>;
using Vector3f = Vector3<RkFloat>;

// Pre instantiation
extern template struct Vector3<RkInt>;
extern template struct Vector3<RkFloat>;

END_RUKEN_NAMESPACE