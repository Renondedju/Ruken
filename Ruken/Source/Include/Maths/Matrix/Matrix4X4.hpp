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

#include "Maths/Vector/Vector.hpp"
#include "Maths/Matrix/SquareMatrix.hpp"

#include "Types/Units/Angle/Degrees.hpp"
#include "Types/Units/Angle/Radians.hpp"

BEGIN_RUKEN_NAMESPACE

class Quaternion;

/**
 * \brief 4x4 Matrix class
 */
class __declspec(novtable) Matrix4X4 final : public SquareMatrix<4>
{
    using Parent = SquareMatrix<4>;

    public:

        #pragma region Constructors

        using Parent::Parent;

        Matrix4X4()                           = default;
        Matrix4X4(Matrix4X4 const& in_matrix) = default;
        Matrix4X4(Matrix4X4&&      in_matrix) = default;
       ~Matrix4X4()                           = default;

        DECLARE_MATRIX_COMPATIBILITY_COPY_CONSTRUCTOR(Matrix4X4, Parent)
        DECLARE_MATRIX_COMPATIBILITY_MOVE_CONSTRUCTOR(Matrix4X4, Parent)

        DECLARE_MATRIX_COMPATIBILITY_COPY_CONSTRUCTOR(Matrix4X4, Parent::Parent)
        DECLARE_MATRIX_COMPATIBILITY_MOVE_CONSTRUCTOR(Matrix4X4, Parent::Parent)

        #pragma endregion

        #pragma region Static Methods

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
        static Matrix4X4 ClipSpace() noexcept;

        /**
         * \brief Generates a translation matrix.
         * \param in_translation Translation of the matrix
         * \return Translation matrix
         */
        [[nodiscard]]
        static Matrix4X4 TranslationMatrix(Vector3f const& in_translation) noexcept;

        /**
         * \brief Creates a rotation matrix for the X axis
         * \param in_angle Angle of the X axis rotation (in radians)
         * \return X axis rotation matrix
         */
        [[nodiscard]]
        static Matrix4X4 RotationMatrixX(Radians in_angle) noexcept;

        /**
         * \brief Creates a rotation matrix for the Y axis
         * \param in_angle Angle of the Y axis rotation (in radians)
         * \return Y axis rotation matrix
         */
        [[nodiscard]]
        static Matrix4X4 RotationMatrixY(Radians in_angle) noexcept;

        /**
         * \brief Creates a rotation matrix for the Z axis
         * \param in_angle Angle of the Z axis rotation (in radians)
         * \return Z axis rotation matrix
         */
        [[nodiscard]]
        static Matrix4X4 RotationMatrixZ(Radians in_angle) noexcept;

        /**
         * \brief Creates a rotation matrix
         *
         * \param in_angle_x X Angle of the matrix
         * \param in_angle_y Y Angle of the matrix
         * \param in_angle_z Z Angle of the matrix
         *
         * \return Rotation matrix
         */
        [[nodiscard]]
        static Matrix4X4 RotationMatrix(
            Radians in_angle_x,
            Radians in_angle_y,
            Radians in_angle_z) noexcept;

        /**
         * \brief Creates a scale matrix
         * \param in_scale Scale of the matrix
         * \return Scale matrix
         */
        [[nodiscard]]
        static Matrix4X4 ScaleMatrix(Vector3f const& in_scale) noexcept;

        /**
         * \brief Creates a perspective projection matrix
         *
         * \param in_fov Field of view
         * \param in_aspect Aspect ratio of the viewport
         * \param in_near Near plane distance (cannot be 0 or negative !)
         * \param in_far Far plane distance
         *
         * \return New perspective projection matrix
         */
        [[nodiscard]]
        static Matrix4X4 PerspectiveProjectionMatrix(
            Degrees in_fov,
            RkFloat in_aspect,
            RkFloat in_near,
            RkFloat in_far) noexcept;

        /**
         * \brief Creates an orthogonal projection matrix
         *
         * \param in_left Left plane distance
         * \param in_right Right plane distance
         * \param in_bottom Bottom plane distance
         * \param in_top Top plane distance
         * \param in_near Near plane distance
         * \param in_far Far plane distance
         *
         * \return Orthogonal projection matrix
         */
        [[nodiscard]]
        static Matrix4X4 OrthogonalProjectionMatrix(
                RkFloat in_left  , RkFloat in_right, 
                RkFloat in_bottom, RkFloat in_top, 
                RkFloat in_near  , RkFloat in_far) noexcept;

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
        static Matrix4X4 LookAtMatrix(
                Vector3f const& in_from,
                Vector3f const& in_to,
                Vector3f const& in_up) noexcept;

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
        static Matrix4X4 ModelMatrix(
                Vector3f   const& in_position,
                Quaternion const& in_rotation,
                Vector3f   const& in_scale) noexcept;

        #pragma endregion

        #pragma region Operators

        Matrix4X4& operator=(Matrix4X4 const& in_other) noexcept = default;
        Matrix4X4& operator=(Matrix4X4&&	    in_other) noexcept = default;

        DECLARE_MATRIX_COMPATIBILITY_COPY_OPERATOR(Matrix4X4, Parent)
        DECLARE_MATRIX_COMPATIBILITY_MOVE_OPERATOR(Matrix4X4, Parent)

        DECLARE_MATRIX_COMPATIBILITY_COPY_OPERATOR(Matrix4X4, Parent::Parent)
        DECLARE_MATRIX_COMPATIBILITY_MOVE_OPERATOR(Matrix4X4, Parent::Parent)

        #pragma endregion
};

END_RUKEN_NAMESPACE