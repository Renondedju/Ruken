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

#include "Types/Units/Angle/Angle.hpp"
#include "Types/Units/Distance/Distance.hpp"

#include "Maths/Vector/DistanceVector3.hpp"
#include "Maths/Matrix/Operations/MatrixAccess.hpp"
#include "Maths/Matrix/Operations/MatrixUtility.hpp"
#include "Maths/Matrix/Operations/MatrixOstream.hpp"
#include "Maths/Matrix/Operations/MatrixInversion.hpp"
#include "Maths/Matrix/Operations/MatrixConversions.hpp"
#include "Maths/Matrix/Operations/MatrixTransposition.hpp"
#include "Maths/Matrix/Operations/MatrixMultiplication.hpp"

BEGIN_RUKEN_NAMESPACE

struct Quaternion;

/**
 * \brief Matrix class.
 *
 * A transformation matrix can perform arbitrary linear 3D transformations (i.e. translation, rotation, scale, shear etc.)
 * and perspective transformations using homogenous coordinates.
 *
 * Matrices in Ruken are column major; i.e. the position of a transformation matrix is in the last column, and the first three columns contain x, y, and z-axes.
 *
 * \tparam TRows Height or number of rows of the matrix
 * \tparam TColumns Width or number of columns of the matrix
 * \note The default constructor creates a zero matrix
 */
template<RkSize TRows, RkSize TColumns>
struct Matrix : MatrixAccess,        MatrixAccess       ::Member<TRows, TColumns>,
                MatrixTransposition, MatrixTransposition::Member<TRows, TColumns>,
                MatrixMultiplication<TRows, TColumns>,
                MatrixConversions   <TRows, TColumns>,
                MatrixUtility       <TRows, TColumns>,
                MatrixOstream       <TRows, TColumns>
{
    #pragma region Members

    RkFloat data[TRows * TColumns] {.0F};

    #pragma endregion

    #pragma region Constructors

    /**
     * \brief Manual constructor
     * \tparam TTypes Value types (must be RkFloats)
     * \param in_values values
     */
    template <typename... TTypes>
    requires (sizeof...(TTypes) == TRows * TColumns && std::conjunction_v<std::is_same<TTypes, RkFloat>...>)
    constexpr Matrix(TTypes... in_values) noexcept:
        data {static_cast<RkFloat>(in_values)...}
    {}

    #pragma endregion
};

/**
 * \brief Square matrix class.
 *
 * A transformation matrix can perform arbitrary linear 3D transformations (i.e. translation, rotation, scale, shear etc.)
 * and perspective transformations using homogenous coordinates.
 *
 * Matrices in Ruken are column major; i.e. the position of a transformation matrix is in the last column, and the first three columns contain x, y, and z-axes.
 *
 * \tparam TSize Width & Height or size of the matrix
 */
template<RkSize TSize>
struct Matrix<TSize, TSize>:
    MatrixAccess,        MatrixAccess       ::Member<TSize, TSize>,
    MatrixInversion,     MatrixInversion    ::Member<TSize>,
    MatrixTransposition, MatrixTransposition::Member<TSize, TSize>,
    MatrixMultiplication<TSize, TSize>,
    MatrixConversions   <TSize, TSize>,
    MatrixUtility       <TSize, TSize>,
    MatrixOstream       <TSize, TSize>
{
    #pragma region Members

    RkFloat data[TSize * TSize] {.0F};

    #pragma endregion

    #pragma region Constructors

    /**
     * \brief Default constructor, creates an identity matrix
     */
    constexpr Matrix() noexcept
    {
        for (RkSize index {0ULL}; index < TSize; ++index)
			data[index * TSize + index] = 1.0F;
    }

    /**
     * \brief Manual constructor
     * \tparam TTypes Value types (must be RkFloats)
     * \param in_values values
     */
    template <typename... TTypes>
    requires (sizeof...(TTypes) == TSize * TSize && std::conjunction_v<std::is_same<TTypes, RkFloat>...>)
    constexpr Matrix(TTypes... in_values) noexcept:
        data {static_cast<RkFloat>(in_values)...}
    {}

    #pragma endregion
};

/**
 * \brief A standard 4x4 transformation matrix
 *
 * A transformation matrix can perform arbitrary linear 3D transformations (i.e. translation, rotation, scale, shear etc.)
 * and perspective transformations using homogenous coordinates.
 *
 * Matrices in Ruken are column major; i.e. the position of a transformation matrix is in the last column, and the first three columns contain x, y, and z-axes.
 * \note The default constructor creates an identity matrix
 */
template<>
struct Matrix<4, 4>:
    MatrixAccess,        MatrixAccess       ::Member<4, 4>,
    MatrixInversion,     MatrixInversion    ::Member<4>,
    MatrixTransposition, MatrixTransposition::Member<4, 4>,
    MatrixMultiplication<4, 4>,
    MatrixConversions   <4, 4>,
    MatrixUtility       <4, 4>,
    MatrixOstream       <4, 4>
{
    #pragma region Members

    // Default constructor creates an identity matrix
    RkFloat data[16] {
        1.F, 0.F, 0.F, 0.F,
        0.F, 1.F, 0.F, 0.F,
        0.F, 0.F, 1.F, 0.F,
        0.F, 0.F, 0.F, 1.F,
    };

    #pragma endregion

    #pragma region Constructors

    constexpr Matrix()              = default;
    constexpr Matrix(Matrix const&) = default;
    constexpr Matrix(Matrix&&)      = default;
             ~Matrix()              = default;

    constexpr Matrix(RkFloat const in_00, RkFloat const in_01, RkFloat const in_02, RkFloat const in_03,
                     RkFloat const in_10, RkFloat const in_11, RkFloat const in_12, RkFloat const in_13,
                     RkFloat const in_20, RkFloat const in_21, RkFloat const in_22, RkFloat const in_23,
                     RkFloat const in_30, RkFloat const in_31, RkFloat const in_32, RkFloat const in_33) noexcept:
        data {in_00, in_01, in_02, in_03,
              in_10, in_11, in_12, in_13,
              in_20, in_21, in_22, in_23,
              in_30, in_31, in_32, in_33}
    {}

    #pragma endregion

    #pragma region Static Methods

    /**
     * \brief Returns the clip space of the Vulkan API.
     *
     * This matrix is generally used in conjunction with a projection matrix to convert an object from the view space to the clip space
     * This matrix allows to do the following transformations:
     *  Position.y = -Position.y;
     *  Position.z = (Position.z + Position.w) / 2.0;
     *
     * \see An example of the use of this matrix: https://github.com/LunarG/VulkanSamples/commit/0dd36179880238014512c0637b0ba9f41febe803
     * 
     * \return Vulkan clip space matrix
     */
    [[nodiscard]]
    static Matrix<4, 4> ClipSpace() noexcept;

    /**
     * \brief Generates a translation matrix.
     * \param in_translation Translation of the matrix
     * \return Translation matrix
     */
    [[nodiscard]]
    static Matrix<4, 4> TranslationMatrix(Vector3m const& in_translation) noexcept;

    /**
     * \brief Creates a rotation matrix for the X axis
     * \param in_angle Angle of the X axis rotation (in radians)
     * \return X axis rotation matrix
     */
    [[nodiscard]]
    static Matrix<4, 4> RotationMatrixX(Radians in_angle) noexcept;

    /**
     * \brief Creates a rotation matrix for the Y axis
     * \param in_angle Angle of the Y axis rotation (in radians)
     * \return Y axis rotation matrix
     */
    [[nodiscard]]
    static Matrix<4, 4> RotationMatrixY(Radians in_angle) noexcept;

    /**
     * \brief Creates a rotation matrix for the Z axis
     * \param in_angle Angle of the Z axis rotation (in radians)
     * \return Z axis rotation matrix
     */
    [[nodiscard]]
    static Matrix<4, 4> RotationMatrixZ(Radians in_angle) noexcept;

    /**
     * \brief Creates a rotation matrix
     *
     * \param in_angle_x X Angle of the matrix
     * \param in_angle_y Y Angle of the matrix
     * \param in_angle_z Z Angle of the matrix
     *
     * \return Rotation matrix
     */
    [[nodiscard]]
    static Matrix<4, 4> RotationMatrix(
        Radians in_angle_x,
        Radians in_angle_y,
        Radians in_angle_z) noexcept;

    /**
     * \brief Creates a scale matrix
     * \param in_scale Scale of the matrix
     * \return Scale matrix
     */
    [[nodiscard]]
    static Matrix<4, 4> ScaleMatrix(Vector3m const& in_scale) noexcept;

    /**
     * \brief Creates a perspective projection matrix
     *
     * \param in_fov Field of view
     * \param in_aspect Aspect ratio of the viewport
     * \param in_near Near plane distance (cannot be 0 or negative !)
     * \param in_far Far plane distance
     *
     * \return New perspective projection matrix
     */
    [[nodiscard]]
    static Matrix<4, 4> PerspectiveProjectionMatrix(
        Degrees in_fov,
        RkFloat in_aspect,
        Meters  in_near,
        Meters  in_far) noexcept;

    /**
     * \brief Creates an orthogonal projection matrix
     *
     * \param in_left Left plane distance
     * \param in_right Right plane distance
     * \param in_bottom Bottom plane distance
     * \param in_top Top plane distance
     * \param in_near Near plane distance
     * \param in_far Far plane distance
     *
     * \return Orthogonal projection matrix
     */
    [[nodiscard]]
    static Matrix<4, 4> OrthogonalProjectionMatrix(
            Meters in_left  , Meters in_right, 
            Meters in_bottom, Meters in_top, 
            Meters in_near  , Meters in_far) noexcept;

    /**
     * \brief Creates model matrix to look at a specific point in the world
     *
     * \param in_from Position to look from
     * \param in_to Position to look at
     * \param in_up Up vector
     *
     * \return Look at matrix
     */
    [[nodiscard]]
    static Matrix<4, 4> LookAtMatrix(
            Vector3m const& in_from,
            Vector3m const& in_to,
            Vector3m const& in_up) noexcept;

    /**
     * \brief Model matrix, also called TRS
     *
     * \param in_position Model position
     * \param in_rotation Model rotation
     * \param in_scale Model scale
     *
     * \return Model (TRS) matrix
     */
    [[nodiscard]]
    static Matrix<4, 4> ModelMatrix(
            Vector3m   const& in_position,
            Quaternion const& in_rotation,
            Vector3m   const& in_scale) noexcept;

    #pragma endregion

    #pragma region Operators

    constexpr Matrix& operator=(Matrix const&) = default;
    constexpr Matrix& operator=(Matrix&&)      = default;

    #pragma endregion
};

END_RUKEN_NAMESPACE