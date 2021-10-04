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

#include "Maths/Matrix/GenericMatrix.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief 1x4 Matrix class
 */
class __declspec(novtable) Matrix1X4 final : public GenericMatrix<1, 4>
{
    using Parent = GenericMatrix<1, 4>;

    public:

        #pragma region Constructors

        using Parent::Parent;

        constexpr Matrix1X4()                           = default;
        constexpr Matrix1X4(Matrix1X4 const& in_matrix) = default;
        constexpr Matrix1X4(Matrix1X4&&      in_matrix) = default;
                 ~Matrix1X4()                           = default;

        DECLARE_MATRIX_COMPATIBILITY_COPY_CONSTRUCTOR(Matrix1X4, Parent)
        DECLARE_MATRIX_COMPATIBILITY_MOVE_CONSTRUCTOR(Matrix1X4, Parent)

        #pragma endregion

        #pragma region Operators

        constexpr Matrix1X4& operator=(Matrix1X4 const& in_other) noexcept = default;
        constexpr Matrix1X4& operator=(Matrix1X4&&	    in_other) noexcept = default;

        DECLARE_MATRIX_COMPATIBILITY_COPY_OPERATOR(Matrix1X4, Parent)
        DECLARE_MATRIX_COMPATIBILITY_MOVE_OPERATOR(Matrix1X4, Parent)

        #pragma endregion
};

END_RUKEN_NAMESPACE