
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
#include "Maths/Matrix/Transformations/MatrixScale.hpp"
#include "Maths/Matrix/Transformations/MatrixLookAt.hpp"
#include "Maths/Matrix/Transformations/MatrixRotation.hpp"
#include "Maths/Matrix/Transformations/MatrixClipSpace.hpp"
#include "Maths/Matrix/Transformations/MatrixRotationX.hpp"
#include "Maths/Matrix/Transformations/MatrixRotationY.hpp"
#include "Maths/Matrix/Transformations/MatrixRotationZ.hpp"
#include "Maths/Matrix/Transformations/MatrixProjections.hpp"
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
    MatrixConversions   <TRows, TColumns>,
    //MatrixInversion     <TRows, TColumns>,
    MatrixMultiplication<TRows, TColumns>,
    MatrixOstream       <TRows, TColumns>,
    MatrixTransposition <TRows, TColumns>,
    MatrixUtility       <TRows, TColumns>,
    // Transformations ---
    MatrixClipSpace     <TRows, TColumns>,
    MatrixLookAt        <TRows, TColumns>,
    MatrixModel         <TRows, TColumns>,
    MatrixProjections   <TRows, TColumns>,
    MatrixRotation      <TRows, TColumns>,
    MatrixRotationX     <TRows, TColumns>,
    MatrixRotationY     <TRows, TColumns>,
    MatrixRotationZ     <TRows, TColumns>,
    MatrixScale         <TRows, TColumns>,
    MatrixTranslation   <TRows, TColumns>
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