#pragma once

#include "Maths/Matrix/MatrixForward.hpp"

#include "Maths/Vector/DistanceVector3.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements translation matrix
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 */
template <RkSize TRows, RkSize TColumns>
struct MatrixTranslation
{
    /**
     * \brief Generates a translation matrix.
     * \param in_translation Translation of the matrix
     * \return Translation matrix
     */
    [[nodiscard]]
    static Matrix<TRows, TColumns> TranslationMatrix(Vector3m const& in_translation) noexcept
        requires (TRows == 4 && TColumns == 4)
    {
        return Matrix<4, 4> {
            1.0F, 0.0F, 0.0F, 0.0F,
            0.0F, 1.0F, 0.0F, 0.0F,
            0.0F, 0.0F, 1.0F, 0.0F,
            in_translation.x, in_translation.y, in_translation.z, 1.0F
        };
    }
};

END_RUKEN_NAMESPACE