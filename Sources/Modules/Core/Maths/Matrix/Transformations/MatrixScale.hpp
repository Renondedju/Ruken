#pragma once

#include "Maths/Matrix/MatrixForward.hpp"
#include "Maths/Vector/DistanceVector3.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements scale matrix
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 */
template <RkSize TRows, RkSize TColumns>
struct MatrixScale
{
    /**
     * \brief Creates a scale matrix
     * \param in_scale Scaling factor
     * \return Scale matrix
     */
    [[nodiscard]]
    static Matrix<TRows, TColumns> ScaleMatrix(Vector3m const& in_scale) noexcept
        requires (TRows == 4 && TColumns == 4)
    {
        return Matrix<4, 4> {
            in_scale.x, 0.0F, 0.0F, 0.0F,
            0.0F, in_scale.y, 0.0F, 0.0F,
            0.0F, 0.0F, in_scale.z, 0.0F,
            0.0F, 0.0F, 0.0F      , 1.0F
        };
    }
};

END_RUKEN_NAMESPACE