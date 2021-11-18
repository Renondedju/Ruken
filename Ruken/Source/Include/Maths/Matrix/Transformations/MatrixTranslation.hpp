#pragma once

#include "Maths/Matrix/MatrixForward.hpp"

#include "Maths/Vector/DistanceVector3.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements translation matrix
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 * \tparam TSfinae Special parameter allowing selection of class specialization to enable or disable some functions 
 */
template <RkSize TRows, RkSize TColumns, typename TSfinae = RkVoid>
struct MatrixTranslation
{};

// Requires a 4x4 matrix
template <RkSize TRows, RkSize TColumns>
struct MatrixTranslation<TRows, TColumns, std::enable_if_t<TRows == 4 && TColumns == 4>>
{
    /**
     * \brief Generates a translation matrix.
     * \param in_translation Translation of the matrix
     * \return Translation matrix
     */
    [[nodiscard]]
    static Matrix<TRows, TColumns> TranslationMatrix(Vector3m const& in_translation) noexcept
    {
        return {
            1.0F,  0.0F,  0.0F,  static_cast<RkFloat>(in_translation.x),
            0.0F,  1.0F,  0.0F,  static_cast<RkFloat>(in_translation.y),
            0.0F,  0.0F,  1.0F,  static_cast<RkFloat>(in_translation.z),
            0.0F,  0.0F,  0.0F,  1.0F
        };
    }
};

END_RUKEN_NAMESPACE