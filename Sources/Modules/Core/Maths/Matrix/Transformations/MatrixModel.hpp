
#pragma once

#include "Maths/Matrix/MatrixForward.hpp"
#include "Maths/Quaternion/Quaternion.hpp"
#include "Maths/Vector/DistanceVector3.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements look at matrix
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 * \tparam TSfinae Special parameter allowing selection of class specialization to enable or disable some functions 
 */
template <RkSize TRows, RkSize TColumns, typename TSfinae = RkVoid>
struct MatrixModel
{};

// Requires at least 3x4 matrix
template <RkSize TRows, RkSize TColumns>
struct MatrixModel<TRows, TColumns, std::enable_if_t<(TRows == 3 || TRows == 4) && TColumns == 4>>
{
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
    static Matrix<TRows, TColumns> ModelMatrix(
            Vector3m   const& in_position,
            Quaternion const& in_rotation,
            Vector3m   const& in_scale) noexcept
    {
        RkFloat const sqr_x (in_rotation.x * in_rotation.x);
        RkFloat const sqr_y (in_rotation.y * in_rotation.y);
        RkFloat const sqr_z (in_rotation.z * in_rotation.z);

        return Matrix<3, 4> {
            static_cast<RkFloat>((1 - 2 * (sqr_y + sqr_z)) * in_scale.x),
            static_cast<RkFloat>(2 * (in_rotation.x * in_rotation.y - in_rotation.z * in_rotation.w) * in_scale.y),
            static_cast<RkFloat>(2 * (in_rotation.x * in_rotation.z + in_rotation.y * in_rotation.w) * in_scale.z),
            static_cast<RkFloat>(in_position.x),

            static_cast<RkFloat>(2 * (in_rotation.x * in_rotation.y + in_rotation.z * in_rotation.w) * in_scale.x),
            static_cast<RkFloat>((1 - 2 * (sqr_x + sqr_z)) * in_scale.y),
            static_cast<RkFloat>(2 * (in_rotation.y * in_rotation.z - in_rotation.x * in_rotation.w) * in_scale.z),
            static_cast<RkFloat>(in_position.y),

            static_cast<RkFloat>(2 * (in_rotation.x * in_rotation.z - in_rotation.y * in_rotation.w) * in_scale.x),
            static_cast<RkFloat>(2 * (in_rotation.y * in_rotation.z + in_rotation.x * in_rotation.w) * in_scale.y),
            static_cast<RkFloat>((1 - 2 * (sqr_x + sqr_y)) * in_scale.z),
            static_cast<RkFloat>(in_position.z)
        };
    }
};

END_RUKEN_NAMESPACE