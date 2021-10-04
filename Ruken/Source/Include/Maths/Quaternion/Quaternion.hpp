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

#include "Maths/Matrix/Matrix3X3.hpp"
#include "Types/Units/Angle/Degrees.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Quaternion class, used to represent 3D orientations and rotations.
 *
 * Unit quaternions provide a convenient mathematical notation for representing orientations and rotations of objects in three dimensions.
 * Compared to Euler angles they are simpler to compose and avoid the problem of gimbal lock.
 * Compared to rotation matrices they are more compact, more numerically stable, and more efficient.
 */
class Quaternion
{
    public:

        #pragma region Members

        RkFloat w {1.0F};
        RkFloat x {0.0F};
        RkFloat y {0.0F};
        RkFloat z {0.0F};

        #pragma endregion

        #pragma region Constructors

        constexpr Quaternion()                          = default;
        constexpr Quaternion(Quaternion const& in_copy) = default;
        constexpr Quaternion(Quaternion&&      in_move) = default;
                 ~Quaternion()                          = default;

        /**
         * \brief Creates a quaternion with the given values
         * 
         * \param in_w w component of the quaternion
         * \param in_x x component of the quaternion
         * \param in_y y component of the quaternion
         * \param in_z z component of the quaternion
         */
        constexpr Quaternion(RkFloat in_w, RkFloat in_x, RkFloat in_y, RkFloat in_z) noexcept;

        /**
         * \brief Creates a quaternion with the given rotations (using euler angles)
         * 
         * \param in_angle_x Angle to rotate around the x axis (in degrees)
         * \param in_angle_y Angle to rotate around the y axis (in degrees)
         * \param in_angle_z Angle to rotate around the z axis (in degrees)
         */
        constexpr Quaternion(Degrees in_angle_x, Degrees in_angle_y, Degrees in_angle_z) noexcept;

        #pragma endregion

        #pragma region Static Methods

        /**
         * \brief Quaternion linear interpolation
         * 
         * \tparam TShortestPath True if the method should lerp from in_lhs to in_rhs using the shortest path, false otherwise
         *
         * \param in_lhs The start quaternion
         * \param in_rhs The end quaternion
         * \param in_ratio The interpolation ratio. Should be between 0 and 1
         * 
         * \return A new quaternion which is the result of the linear interpolation between in_lhs and in_rhs at in_ratio
         */
        template <RkBool TShortestPath = true>
        static constexpr Quaternion Lerp(Quaternion const& in_lhs, Quaternion const& in_rhs, float in_ratio) noexcept;

        /**
         * \brief Quaternion spherical interpolation
         *
         * \tparam TShortestPath True if the method should slerp from in_lhs to in_rhs using the shortest path, false otherwise
         *
         * \param in_lhs The start quaternion
         * \param in_rhs The end quaternion
         * \param in_ratio The interpolation ratio. Should be between 0 and 1
         *
         * \return A new quaternion which is the result of the spherical linear interpolation between in_lhs and in_rhs at in_ratio
         */
        template <RkBool TShortestPath = true>
        static constexpr Quaternion Slerp(Quaternion const& in_lhs, Quaternion const& in_rhs, float in_ratio) noexcept;

        /**
         * \brief Computes the dot product between two quaternions
         * 
         * \param in_lhs Left hand side quaternion
         * \param in_rhs Right hand side quaternion
         * 
         * \return The resulting dot product
         */
        static constexpr RkFloat Dot(Quaternion const& in_lhs, Quaternion const& in_rhs) noexcept;

        /**
         * \brief Normalizes a quaternion
         *
         * \param in_quaternion Quaternion to be normalized
         * \return Normalized quaternion
         */
        static constexpr Quaternion Normalize(Quaternion const& in_quaternion) noexcept;

        /**
         * \brief Computes the square length of the quaternion
         *
         * \param in_quaternion Quaternion instance
         * \return Squared length of the quaternion
         */
        static constexpr RkFloat SqrLength(Quaternion const& in_quaternion) noexcept;

        /**
         * \brief Computes the length of the quaternion
         *
         * \param in_quaternion Quaternion instance
         * \return Length of the quaternion
         */
        static constexpr RkFloat Length(Quaternion const& in_quaternion) noexcept;

        /**
         * \brief Inverts the passed quaternion
         *
         * \param in_quaternion Quaternion instance
         * \return Inverted quaternion
         */
        static constexpr Quaternion Invert(Quaternion const& in_quaternion) noexcept;

        /**
         * \brief Multiplies 2 quaternions together. This effectively represents composing the two rotations.
         * \param in_lhs Left hand side quaternion
         * \param in_rhs Right hand side quaternion
         * \return Resulting quaternion
         */
        static constexpr Quaternion Multiply(Quaternion const& in_lhs, Quaternion const& in_rhs) noexcept;

        /**
         * \brief Scales all components of a quaternion by a coefficient
         * \param in_quaternion Quaternion to scale
         * \param in_coefficient Coefficient
         * \return Scaled quaternion
         */
        static constexpr Quaternion Scale(Quaternion const& in_quaternion, RkFloat in_coefficient) noexcept;

        /**
         * \brief Adds all the components of the quaternions together
         * \param in_lhs Left hand side quaternion
         * \param in_rhs Right hand side quaternion
         * \return Resulting quaternion 
         */
        static constexpr Quaternion Add(Quaternion const& in_lhs, Quaternion const& in_rhs) noexcept; 

        /**
         * \brief Computes an homogenous rotation 3x3 matrix for a given quaternion
         * \param in_quaternion Quaternion to compute the rotation matrix for
         * \return Homogenous 3x3 rotation matrix
         */
        static constexpr Matrix3X3 RotationMatrix(Quaternion const& in_quaternion) noexcept;

        #pragma endregion

        #pragma region Operators

        constexpr Quaternion& operator=(Quaternion const& in_copy) = default;
        constexpr Quaternion& operator=(Quaternion&&	  in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE