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

#include "Types/FundamentalTypes.hpp"
#include "Build/Attributes/EmptyBases.hpp"

#include "Maths/Matrix/Operations/MatrixAccess.hpp"
#include "Maths/Matrix/Operations/MatrixUtility.hpp"
#include "Maths/Matrix/Operations/MatrixOstream.hpp"
#include "Maths/Matrix/Operations/MatrixInversion.hpp"
#include "Maths/Matrix/Operations/MatrixConversions.hpp"
#include "Maths/Matrix/Operations/MatrixTransposition.hpp"
#include "Maths/Matrix/Operations/MatrixMultiplication.hpp"

#include "Maths/Matrix/Transformations/MatrixModel.hpp"
#include "Maths/Matrix/Transformations/MatrixRotation.hpp"
#include "Maths/Matrix/Transformations/MatrixRotationX.hpp"
#include "Maths/Matrix/Transformations/MatrixRotationY.hpp"
#include "Maths/Matrix/Transformations/MatrixRotationZ.hpp"
#include "Maths/Matrix/Transformations/MatrixTranslation.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Matrix class.
 *
 * A transformation matrix can perform arbitrary linear 3D transformations (i.e. translation, rotation, scale, shear etc.)
 * and perspective transformations using homogenous coordinates.
 *
 * Matrices in Ruken are column major;
 * i.e. the position of a transformation matrix is in the last column, and the first three columns contain x, y, and z-axes.
 *
 * \tparam TRows Height or number of rows of the matrix
 * \tparam TColumns Width or number of columns of the matrix
 */
template<RkSize TRows, RkSize TColumns>
struct RUKEN_EMPTY_BASES Matrix:
    MatrixAccess       ::Member<TRows, TColumns>,
    MatrixTransposition <TRows, TColumns>,
    //MatrixInversion     <TRows, TColumns>,
    MatrixMultiplication<TRows, TColumns>,
    MatrixConversions   <TRows, TColumns>,
    MatrixUtility       <TRows, TColumns>,
    MatrixOstream       <TRows, TColumns>,
    MatrixTranslation   <TRows, TColumns>,
    MatrixRotation      <TRows, TColumns>,
    MatrixRotationX     <TRows, TColumns>,
    MatrixRotationY     <TRows, TColumns>,
    MatrixRotationZ     <TRows, TColumns>,
    MatrixModel         <TRows, TColumns>
{
    #pragma region Members

    RkFloat data[TRows * TColumns];

    #pragma endregion

    #pragma region Constructors

    /**
     * \brief Default constructor
     *
     * Initializes the matrices by filling the middle diagonal with ones,
     * effectively creating an identity matrix if the matrix is a square 
     */
    constexpr Matrix() noexcept:
        data {0.F}
    {
        for (RkSize index {0ULL}; index < std::min(TRows, TColumns); ++index)
			data[index * TColumns + index] = 1.F;
    }

    /**
     * \brief Manual constructor
     * \tparam TTypes Value types
     * \param in_values values
     */
    template <typename... TTypes>
    requires (sizeof...(TTypes) == TRows * TColumns)
    constexpr Matrix(TTypes... in_values) noexcept:
        data {static_cast<RkFloat>(in_values)...}
    {}

    #pragma endregion
};

END_RUKEN_NAMESPACE