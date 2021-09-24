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

#pragma region Constructors

template <ArithmeticType TType>
constexpr Vector2<TType>::Vector2(TType const in_x, TType const in_y) noexcept:
    x {in_x},
    y {in_y}
{}

template <ArithmeticType TType>
constexpr Vector2<TType>::Vector2(BaseVector<Vector2<TType>, 2UI64, TType> const& in_copy) noexcept
{
    *this = static_cast<Vector2>(in_copy);
}

template <ArithmeticType TType>
constexpr Vector2<TType>::Vector2(BaseVector<Vector2<TType>, 2, TType>&& in_move) noexcept
{
    *this = std::forward<Vector2>(static_cast<Vector2&&>(in_move));
}

#pragma endregion

#pragma region Operators

template <ArithmeticType TType>
constexpr Vector2<TType>& Vector2<TType>::operator=(BaseVector<Vector2<TType>, 2, TType> const& in_copy) noexcept
{
    return *this = static_cast<Vector2>(in_copy);
}

template <ArithmeticType TType>
constexpr Vector2<TType>& Vector2<TType>::operator=(BaseVector<Vector2<TType>, 2, TType>&& in_move) noexcept
{
    return *this = std::forward<Vector2>(static_cast<Vector2&&>(in_move));
}

#pragma endregion
