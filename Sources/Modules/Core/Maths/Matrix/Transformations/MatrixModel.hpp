#pragma once

#include "Maths/Matrix/MatrixForward.hpp"
#include "Maths/Quaternion/Quaternion.hpp"
#include "Maths/Vector/DistanceVector3.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * @brief Implements model matrix, also called TRS (Transform Rotation Scale)
 * @tparam TRows    Number of rows of the matrix
 * @tparam TColumns Number of columns of the matrix
 */
template <RkSize TRows, RkSize TColumns>
struct MatrixModel
{
    /**
     * @brief Model matrix, also called TRS (Transform Rotation Scale)
     *
     * @param in_position Model position
     * @param in_rotation Model rotation
     * @param in_scale	  Model scale
     */
    [[nodiscard]]
    static Matrix<TRows, TColumns> ModelMatrix(
            Vector3m   const& in_position,
            Quaternion const& in_rotation,
            Vector3m   const& in_scale) noexcept
		requires (TRows == 4 && TColumns == 4)
    {
    	return Matrix<TRows, TColumns>::ScaleMatrix		 (in_scale)    *
			   Matrix<TRows, TColumns>::RotationMatrix   (in_rotation) *
			   Matrix<TRows, TColumns>::TranslationMatrix(in_position);
    }
};

END_RUKEN_NAMESPACE

//#include "Core/Maths/Matrix/Transformations/MatrixModel.inl"