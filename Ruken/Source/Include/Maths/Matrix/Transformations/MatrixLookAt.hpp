
#pragma once

#include "Maths/Matrix/MatrixForward.hpp"
#include "Maths/Vector/DistanceVector3.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements look at matrix
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 * \tparam TSfinae Special parameter allowing selection of class specialization to enable or disable some functions 
 */
template <RkSize TRows, RkSize TColumns, typename TSfinae = RkVoid>
struct MatrixLookAt
{};

// Requires at least a 3x4 matrix
template <RkSize TRows, RkSize TColumns>
struct MatrixLookAt<TRows, TColumns, std::enable_if_t<(TRows == 3 || TRows == 4) && TColumns == 4>>
{
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
    static Matrix<TRows, TRows> LookAtMatrix(
            Vector3m const& in_from,
            Vector3m const& in_to,
            Vector3m const& in_up) noexcept
    {
        Vector3m const forward   ((in_to - in_from   ).Normalized());
        Vector3m const right     (in_up.Cross(forward).Normalized());
        Vector3m const up        (forward.Cross(right));

        return Matrix<3, 4> {
            static_cast<RkFloat>(right.x), static_cast<RkFloat>(up.x), static_cast<RkFloat>(forward.x), static_cast<RkFloat>(in_from.x),
            static_cast<RkFloat>(right.y), static_cast<RkFloat>(up.y), static_cast<RkFloat>(forward.y), static_cast<RkFloat>(in_from.y),
            static_cast<RkFloat>(right.z), static_cast<RkFloat>(up.z), static_cast<RkFloat>(forward.z), static_cast<RkFloat>(in_from.z)
        };
    }
};

END_RUKEN_NAMESPACE