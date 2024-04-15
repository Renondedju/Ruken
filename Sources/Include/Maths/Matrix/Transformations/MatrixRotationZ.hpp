#pragma once

#include "Maths/Trigonometry.hpp"

#include "Maths/Matrix/MatrixForward.hpp"

#include "Types/Units/Angle/Angle.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements the rotation matrix on the z axis
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 * \tparam TSfinae Special parameter allowing selection of class specialization to enable or disable some functions 
 */
template <RkSize TRows, RkSize TColumns, typename TSfinae = RkVoid>
struct MatrixRotationZ
{};

// Z rotation matrix requires at least a 2x2 matrix
template <RkSize TRows, RkSize TColumns>
struct MatrixRotationZ<TRows, TColumns, std::enable_if_t<TRows >= 2 && TColumns >= 2>>
{
    /**
     * \brief Creates a rotation matrix for the Z axis
     * \param in_angle Angle of the Z axis rotation (in radians)
     * \return Z axis rotation matrix
     */
    [[nodiscard]]
    static Matrix<TRows, TColumns> RotationMatrixZ(Radians const in_angle) noexcept
    {
        RkFloat const cos = Cos(in_angle);
        RkFloat const sin = Sin(in_angle);

        return Matrix<2, 2> {
             cos,  -sin,
             sin,   cos
        };
    }
};

END_RUKEN_NAMESPACE