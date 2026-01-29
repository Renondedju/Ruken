#pragma once

#include "Types/FundamentalTypes.hpp"
#include "Core/Maths/Quaternion/Quaternion.hpp"
#include "Core/Maths/Vector/DistanceVector.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * @brief Matrix class.
 *
 * A transformation matrix can perform arbitrary linear 3D transformations (i.e. translation, rotation, scale, shear etc.)
 * and perspective transformations using homogenous coordinates.
 *
 * Matrices in Ruken are column major;
 * i.e. the position of a transformation matrix is in the last column, and the first three columns contain x, y, and z-axes.
 *
 * @tparam TRows Height or number of rows of the matrix
 * @tparam TColumns Width or number of columns of the matrix
 */
template<RkSize TRows, RkSize TColumns>
struct Matrix
    //MatrixInversion     <TRows, TColumns>,
{
    #pragma region Members

    static constexpr RkSize rows      = TRows;
    static constexpr RkSize columns   = TColumns;
    static constexpr RkSize elements  = TRows * TColumns;
    static constexpr RkBool is_square = TRows == TColumns;

    RkFloat data[elements];

    #pragma endregion

    #pragma region Constructors

    /**
     * @brief Default constructor
     *
     * Initializes the matrices by filling the middle diagonal with ones,
     * effectively creating an identity matrix if the matrix is a square.
     */
    constexpr Matrix() noexcept;

    /**
     * @brief Manual constructor
     * @tparam TTypes Value types
     * @param in_values values
     */
    template <typename... TTypes> requires (sizeof...(TTypes) == TRows * TColumns)
    explicit constexpr Matrix(TTypes... in_values) noexcept;

    #pragma endregion

    #pragma region Static Methods

    /**
     * @brief Creates model matrix to look at a specific point in the world.
     *
     * @param in_from Position to look from.
     * @param in_to   Position to look at.
     * @param in_up   Up vector.
     * @return Look at matrix
     */
    [[nodiscard]]
    constexpr static Matrix LookAtMatrix(
            Vector3m const& in_from,
            Vector3m const& in_to,
            Vector3m const& in_up) noexcept
        requires ((TRows == 3 || TRows == 4) && TColumns == 4);

    /**
     * @brief Model matrix, also called TRS (Transform Rotation Scale)
     *
     * @param in_position Model position
     * @param in_rotation Model rotation
     * @param in_scale	  Model scale
     */
    [[nodiscard]]
    constexpr static Matrix ModelMatrix(
            Vector3m   const& in_position,
            Quaternion const& in_rotation,
            Vector3m   const& in_scale) noexcept
        requires (TRows == 4 && TColumns == 4);

    /**
     * @brief Creates a perspective projection matrix
     *
     * @param in_fov Field of view
     * @param in_aspect Aspect ratio of the viewport
     * @param in_near Near plane distance (cannot be 0 or negative !)
     * @param in_far Far plane distance
     *
     * @return New perspective projection matrix
     */
    [[nodiscard]]
    constexpr static Matrix PerspectiveProjectionMatrix(
            Radians in_fov,
            RkFloat in_aspect,
            Meters in_near,
            Meters in_far) noexcept
        requires (TRows == 4 && TColumns == 4);

    /**
     * @brief Creates an orthogonal projection matrix
     *
     * @param in_left Left plane distance
     * @param in_right Right plane distance
     * @param in_bottom Bottom plane distance
     * @param in_top Top plane distance
     * @param in_near Near plane distance
     * @param in_far Far plane distance
     *
     * @return Orthogonal projection matrix
     */
    [[nodiscard]]
    constexpr static Matrix OrthogonalProjectionMatrix(
            Meters in_left  , Meters in_right,
            Meters in_bottom, Meters in_top,
            Meters in_near  , Meters in_far) noexcept
        requires (TRows == 4 && TColumns == 4);

    /**
     * @brief Creates a 3D rotation matrix.
     *
     * @param in_angle_x X Angle of the matrix.
     * @param in_angle_y Y Angle of the matrix.
     * @param in_angle_z Z Angle of the matrix.
     * @return Rotation matrix.
     */
    [[nodiscard]]
    constexpr static Matrix RotationMatrix3D(Radians in_angle_x, Radians in_angle_y, Radians in_angle_z) noexcept
        requires (TRows >= 3 && TColumns >= 3);

    /**
     * @brief Axis angle 3D rotation matrix.
     * @param in_axis Axis to rotate around (must be normalized).
     * @param in_angle Angle of the rotation.
     * @return Rotation matrix.
     */
    [[nodiscard]]
    constexpr static Matrix RotationMatrix3D(Vector3m const& in_axis, Radians in_angle) noexcept
        requires (TRows >= 3 && TColumns >= 3);

    /**
     * @brief Creates a 3D rotation matrix.
     * @return Rotation matrix.
     */
    [[nodiscard]]
    constexpr static Matrix RotationMatrix3D(Quaternion in_value) noexcept
        requires (TRows >= 3 && TColumns >= 3);

    /**
     * @brief Creates a rotation matrix around the X-axis.
     * @param in_angle Angle of the X-axis rotation (in radians).
     * @return X-axis rotation matrix.
     */
    [[nodiscard]]
    constexpr static Matrix RotationMatrixX(Radians in_angle) noexcept
        requires (TRows >= 3 && TColumns >= 3);

    /**
     * @brief Creates a rotation matrix around the Y-axis.
     * @param in_angle Angle of the Y-axis rotation (in radians).
     * @return Y-axis rotation matrix.
     */
    [[nodiscard]]
    constexpr static Matrix RotationMatrixY(Radians in_angle) noexcept
        requires (TRows >= 3 && TColumns >= 3);

    /**
     * @brief Creates a rotation matrix around the Z-axis.
     * @param in_angle Angle of the Z-axis rotation (in radians).
     * @return Z-axis rotation matrix.
     */
    [[nodiscard]]
    constexpr static Matrix RotationMatrixZ(Radians in_angle) noexcept
        requires (TRows >= 2 && TColumns >= 2);

    /**
     * @brief Creates a scale matrix
     * @param in_scale Scaling factor
     * @return Scale matrix
     */
    template<RkSize TSize>
    [[nodiscard]]
    constexpr static Matrix ScaleMatrix(Vector<TSize, Meters> const& in_scale) noexcept
        requires (is_square && TRows >= TSize);

    /**
     * @brief Generates a translation matrix.
     * @param in_translation Translation of the matrix
     * @return Translation matrix
     */
    [[nodiscard]]
    constexpr static Matrix TranslationMatrix(Vector3m const& in_translation) noexcept
        requires (TRows == 4 && TColumns == 4);

    #pragma endregion

    #pragma region Operators

    /**
     * @brief Multiplies two matrices
     * @tparam TOtherRows Height or number of rows of the other matrix
     * @tparam TOtherColumns Width or number of columns of the other matrix
     * @param in_matrix Right hand side operand
     * @return Multiplied matrix
     */
    template <RkSize TOtherRows, RkSize TOtherColumns>
        requires (TColumns == TOtherRows)
    constexpr Matrix<TRows, TOtherColumns> operator*(Matrix<TOtherRows, TOtherColumns> const& in_matrix) const noexcept;

    /**
     * @brief Multiplies this matrix with another and assigns the result into this matrix (A = A * B).
     * @param in_matrix Other matrix to be multiplied with.
     * @return Matrix instance.
     */
    constexpr Matrix& operator*=(Matrix const& in_matrix) noexcept;

    /**
     * @brief Matrix conversion operator.
     * @note This operator is only explicit if one of the new sizes of the matrix is smaller than the original one.
     * @tparam TNewRows New height or number of rows of the matrix.
     * @tparam TNewColumns New width or number of columns of the matrix.
     */
    template <RkSize TNewRows, RkSize TNewColumns> explicit(TNewRows < TRows || TNewColumns < TColumns)
    constexpr operator Matrix<TNewRows, TNewColumns>() const noexcept;

    /**
     * Access operator.
     * @param in_row Row index [0 to TRows).
     * @param in_column Column index [0 to TRows).
     * @return Matrix element for the requested position.
     */
    constexpr RkFloat const& operator[](RkSize in_row, RkSize in_column) const noexcept;
    constexpr RkFloat&       operator[](RkSize in_row, RkSize in_column)       noexcept;

    #pragma endregion
};

using Matrix4x4 = Matrix<4, 4>;

END_RUKEN_NAMESPACE

#include "Core/Maths/Matrix/Matrix.inl"