#pragma once

#include "Maths/Trigonometry.hpp"

#include "Maths/Matrix/MatrixForward.hpp"

#include "Types/Units/Angle/Angle.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements the rotation matrix on the x axis
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 * \tparam TSfinae Special parameter allowing selection of class specialization to enable or disable some functions 
 */
template <RkSize TRows, RkSize TColumns, typename TSfinae = RkVoid>
struct MatrixRotationX
{};

// X rotation matrix requires at least a 3x3 matrix
template <RkSize TRows, RkSize TColumns>
struct MatrixRotationX<TRows, TColumns, std::enable_if_t<TRows >= 3 && TColumns >= 3>>
{
    /**
     * \brief Creates a rotation matrix for the X axis
     * \param in_angle Angle of the X axis rotation (in radians)
     * \return X axis rotation matrix
     */
    [[nodiscard]]
    static Matrix<TRows, TColumns> RotationMatrixX(Radians const in_angle) noexcept
    {
        RkFloat const cos = Cos(in_angle);
        RkFloat const sin = Sin(in_angle);

        return Matrix<3, 3> {
            1.0F,  0.0F,  0.0F,
            0.0F,  cos,   -sin,
            0.0F,  sin,    cos
        };
    }
};

END_RUKEN_NAMESPACE