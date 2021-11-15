
#pragma once

#include "Maths/Matrix/MatrixForward.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements the clip space matrix
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 * \tparam TSfinae Special parameter allowing selection of class specialization to enable or disable some functions 
 */
template <RkSize TRows, RkSize TColumns, typename TSfinae = RkVoid>
struct MatrixClipSpace
{};

// Requires a 4x4 matrix
template <RkSize TRows, RkSize TColumns>
struct MatrixClipSpace<TRows, TColumns, std::enable_if_t<TRows == 4 && TColumns == 4>>
{
    /**
     * \brief Returns the clip space of the Vulkan API.
     *
     * This matrix is generally used in conjunction with a projection matrix to convert an object from the view space to the clip space
     * This matrix allows to do the following transformations:
     *  Position.y = -Position.y;
     *  Position.z = (Position.z + Position.w) / 2.0;
     *
     * \see An example of the use of this matrix: https://github.com/LunarG/VulkanSamples/commit/0dd36179880238014512c0637b0ba9f41febe803
     * 
     * \return Vulkan clip space matrix
     */
    [[nodiscard]]
    static Matrix<TRows, TColumns> ClipSpace() noexcept
    {
        return Matrix<4, 4> {
            1.0F,  0.0F,  0.0F,  0.0F,
            0.0F, -1.0F,  0.0F,  0.0F,
            0.0F,  0.0F,  0.5F,  0.5F,
            0.0F,  0.0F,  0.0F,  1.0F
        };
    }
};

END_RUKEN_NAMESPACE