#pragma once

#include "Maths/Matrix/MatrixForward.hpp"
#include "Maths/Vector/DistanceVector3.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements look at matrix
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 */
template <RkSize TRows, RkSize TColumns>
struct MatrixLookAt
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
    static Matrix<TRows, TColumns> LookAtMatrix(
            Vector3m const& in_from,
            Vector3m const& in_to,
            Vector3m const& in_up) noexcept
        requires ((TRows == 3 || TRows == 4) && TColumns == 4)
    {
        Vector3m const forward{(in_to - in_from   ).Normalized()};
        Vector3m const right  {forward.Cross(in_up).Normalized()};
        Vector3m const up     {right.Cross(forward)};

        return Matrix<3, 4> {
             right  .x,  right  .y,  right  .z, -right  .Dot(in_from),
             up     .x,  up     .y,  up     .z, -up     .Dot(in_from),
            -forward.x, -forward.y, -forward.z,  forward.Dot(in_from)
        };
    }
};

END_RUKEN_NAMESPACE