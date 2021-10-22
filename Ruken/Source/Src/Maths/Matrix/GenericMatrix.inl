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

template <RkSize TRows, RkSize TColumns>
constexpr GenericMatrix<TRows, TColumns>::GenericMatrix(RkFloat const (&in_values)[TRows * TColumns]) noexcept
{
    std::memcpy(&data, &in_values, sizeof data);
}

#pragma endregion

#pragma region Methods

template <RkSize TRows, RkSize TColumns>
constexpr GenericMatrix<TColumns, TRows> GenericMatrix<TRows, TColumns>::GetTransposed() const noexcept
{
    GenericMatrix<TColumns, TRows> transposed;

    for (RkSize i = 0ULL; i < elements / 2ULL; ++i)
        transposed.data[i] = data[elements - i];

    return transposed;
}

template <RkSize TRows, RkSize TColumns>
constexpr RkFloat const& GenericMatrix<TRows, TColumns>::At(RkSize const in_row, RkSize const in_column) const noexcept
{
    return data[TRows * in_row + in_column];
}

template <RkSize TRows, RkSize TColumns>
constexpr RkFloat& GenericMatrix<TRows, TColumns>::At(RkSize const in_row, RkSize const in_column) noexcept
{
    return data[TRows * in_row + in_column];
}

#pragma endregion