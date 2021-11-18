#pragma once

#include "Maths/Matrix/MatrixForward.hpp"

#include "Maths/Vector/DistanceVector3.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements scale matrix
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 * \tparam TSfinae Special parameter allowing selection of class specialization to enable or disable some functions 
 */
template <RkSize TRows, RkSize TColumns, typename TSfinae = RkVoid>
struct MatrixScale
{};

// Requires a 4x4 matrix
template <RkSize TRows, RkSize TColumns>
struct MatrixScale<TRows, TColumns, std::enable_if_t<TRows == 4 && TColumns == 4>>
{
    /**
     * \brief Creates a scale matrix
     * \param in_scale Scaling factor
     * \return Scale matrix
     */
    [[nodiscard]]
    static Matrix<TRows, TColumns> ScaleMatrix(Vector3m const& in_scale) noexcept
    {
        return {
            static_cast<RkFloat>(in_scale.x), 0.0F, 0.0F, 0.0F,
            0.0F, static_cast<RkFloat>(in_scale.y), 0.0F, 0.0F,
            0.0F, 0.0F, static_cast<RkFloat>(in_scale.z), 0.0F,
            0.0F, 0.0F, 0.0F                            , 1.0F
        };
    }
};

END_RUKEN_NAMESPACE