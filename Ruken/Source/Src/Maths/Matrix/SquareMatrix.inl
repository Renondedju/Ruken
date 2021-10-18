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

template<RkSize TSize>
constexpr SquareMatrix<TSize> SquareMatrix<TSize>::Identity() noexcept
{
    SquareMatrix<TSize> identity_matrix;

    for (RkSize i = 0ULL; i < TSize; ++i)
        identity_matrix.At(i, i) = 1.0F;

    return identity_matrix;
}

template<RkSize TSize>
constexpr SquareMatrix<TSize>& SquareMatrix<TSize>::Transpose() noexcept
{
    for (RkSize i = 0ULL; i < Parent::Elements() / 2; ++i)
        std::swap(Parent::data[i], Parent::data[Parent::Elements() - i]);

    return *this;
}

template<RkSize TSize>
constexpr void SquareMatrix<TSize>::LUDecomposition(SquareMatrix<TSize>& out_l_matrix,
                                                    SquareMatrix<TSize>& out_u_matrix) const noexcept
{
    // Decomposing matrix into Upper and Lower
    // Triangular matrix 
    for (RkSize i = 0ULL; i < TSize; ++i)
    {
        // Upper Triangular 
        for (RkSize k = i; k < TSize; ++k)
        {
            // Summation of L(i, j) * U(j, k) 
            RkFloat sum = .0F; 
            for (RkSize j = 0ULL; j < i; ++j) 
                sum += out_l_matrix.At(i, j) * out_u_matrix.At(j, k); 

            // Evaluating U(i, k) 
            out_u_matrix.At(i, k) = Parent::At(i, k) - sum; 
        }

        // Lower Triangular 
        for (RkSize k = i; k < TSize; ++k)
        {
            if (i == k) 
                out_l_matrix.At(i, i) = 1.0F; // Diagonal as 1 
            else
            { 
                // Summation of L(k, j) * U(j, i) 
                RkFloat sum = .0F; 
                for (int j = 0; j < i; j++) 
                    sum += out_l_matrix.At(k, j) * out_u_matrix.At(j, i); 

                // Evaluating L(k, i) 
                out_l_matrix.At(k, i) = (Parent::At(k, i) - sum) / out_u_matrix.At(i, i); 
            } 
        }
    }
}

template<RkSize TSize>
constexpr Vector<TSize, RkFloat> SquareMatrix<TSize>::ForwardSubstitution(Vector<TSize, RkFloat> const& in_vector) const noexcept
{
    Vector<TSize, RkFloat> output;

    RkFloat sum;
    for(RkSize row = 0; row < TSize; ++row)
    {
        sum = 0;
        for(RkSize column = 0; column < row; ++column)
            sum = sum + Parent::At(row, column) * output[column];

        output[row] = (in_vector[row] - sum) / Parent::At(row, row);
    }

    return output;
}

// FIXME: Not working yet
template<RkSize TSize>
constexpr Vector<TSize, RkFloat> SquareMatrix<TSize>::BackwardSubstitution(Vector<TSize, RkFloat> const& in_vector) const noexcept
{
    Vector<TSize, RkFloat> output;

    output[TSize-1] = in_vector[TSize-1] / Parent::At(TSize-1, TSize-1);

    for (int64_t row = TSize-2; row >= 0; --row)
    {
        output[row] = in_vector[row];
        for (RkSize column = row + 1; column < TSize; ++column)
            output[row] -= Parent::At(column, row) * output[column];

        output[row] = output[row] / Parent::At(row, row);
    }

    return output;
}

// FIXME: Not working yet
template<RkSize TSize>
constexpr SquareMatrix<TSize> SquareMatrix<TSize>::GetInverted() const noexcept
{
    SquareMatrix<TSize> inverted_matrix;
    SquareMatrix<TSize> const identity = Identity();

    // Computing the LU matrices
    SquareMatrix<TSize> l_matrix;
    SquareMatrix<TSize> u_matrix;
    LUDecomposition(l_matrix, u_matrix);

    for (RkSize column = 0ULL; column < TSize; ++column)
    {
        Vector<TSize, RkFloat> vector = u_matrix.BackwardSubstitution(
            l_matrix.ForwardSubstitution(
                GenericVector<TSize>(&identity.data[column * TSize])
            )
        );

        memcpy(&inverted_matrix.data[column * TSize], vector.data, sizeof vector);
    }

    return inverted_matrix;
}