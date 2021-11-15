/**
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

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