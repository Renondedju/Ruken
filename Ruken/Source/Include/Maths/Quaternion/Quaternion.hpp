
#pragma once

//#include "Maths/Matrix/Matrix.hpp"
#include "Types/Units/Angle/Angle.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Quaternion class, used to represent 3D orientations and rotations.
 *
 * Unit quaternions provide a convenient mathematical notation for representing orientations and rotations of objects in three dimensions.
 * Compared to Euler angles they are simpler to compose and avoid the problem of gimbal lock.
 * Compared to rotation matrices they are more compact, more numerically stable, and more efficient.
 */
struct Quaternion
{
    #pragma region Members

    RkFloat w {1.0F};
    RkFloat x {0.0F};
    RkFloat y {0.0F};
    RkFloat z {0.0F};

    #pragma endregion

    #pragma region Constructors

    constexpr Quaternion()                  = default;
    constexpr Quaternion(Quaternion const&) = default;
    constexpr Quaternion(Quaternion&&)      = default;
             ~Quaternion()                  = default;

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
     * \param in_angle_x Angle to rotate around the x axis
     * \param in_angle_y Angle to rotate around the y axis
     * \param in_angle_z Angle to rotate around the z axis
     */
    Quaternion(Radians in_angle_x, Radians in_angle_y, Radians in_angle_z) noexcept;

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Inverts the rotation
     * \returns Quaternion instance
     */
    constexpr Quaternion& Invert() noexcept;

    /**
     * \brief Computes the square length of the quaternion
     * \return Squared length of the quaternion
     */
    [[nodiscard]]
    constexpr RkFloat SqrLength() const noexcept;

    /**
     * \brief Computes the length of the quaternion
     * \return Length of the quaternion
     */
    [[nodiscard]]
    RkFloat Length() const noexcept;

    /**
     * \brief Sets the magnitude of the quaternion to 1 while keeping the original orientation
     * \return Quaternion instance
     */
    Quaternion& Normalized() noexcept;

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
    static Quaternion Lerp(Quaternion const& in_lhs, Quaternion const& in_rhs, RkFloat in_ratio) noexcept;

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
    static Quaternion Slerp(Quaternion const& in_lhs, Quaternion const& in_rhs, RkFloat in_ratio) noexcept;

    /**
     * \brief Computes the dot product between two quaternions
     * 
     * \param in_lhs Left hand side quaternion
     * \param in_rhs Right hand side quaternion
     * 
     * \return The resulting dot product
     */
    static RkFloat Dot(Quaternion const& in_lhs, Quaternion const& in_rhs) noexcept;

    /**
     * \brief Scales all components of a quaternion by a coefficient
     * \param in_quaternion Quaternion to scale
     * \param in_coefficient Coefficient
     * \return Scaled quaternion
     */
    static Quaternion Scale(Quaternion const& in_quaternion, RkFloat in_coefficient) noexcept;

    /**
     * \brief Adds all the components of the quaternions together
     * \param in_lhs Left hand side quaternion
     * \param in_rhs Right hand side quaternion
     * \return Resulting quaternion 
     */
    static Quaternion Add(Quaternion const& in_lhs, Quaternion const& in_rhs) noexcept; 

    #pragma endregion

    #pragma region Operators

    /**
     * \brief Multiplies 2 quaternions together. This effectively represents combines the two rotations.
     *
     * Rotating by the product lhs * rhs is the same as applying the two rotations in sequence: lhs first and then rhs,
     * relative to the reference frame resulting from lhs rotation. Note that this means rotations are not commutative,
     * so lhs * rhs does not give the same rotation as rhs * lhs.
     *
     * \param in_other Right hand side quaternion
     * \return Resulting rotation
     */
    [[nodiscard]]
    constexpr Quaternion operator*(Quaternion const& in_other) const noexcept;

    constexpr Quaternion& operator=(Quaternion const& in_copy) = default;
    constexpr Quaternion& operator=(Quaternion&&	  in_move) = default;

    #pragma endregion
};

template <>
struct Constants<Quaternion>
{
    /**
     * \brief The identity rotation.
     * This quaternion corresponds to "no rotation"
     *
     * \note This constant is for code clarity only since the default constructor of
     * \note a quaternion already is the identity quaternion
     */
    static constexpr Quaternion identity = Quaternion();
};

#include "Maths/Quaternion/Quaternion.inl"

END_RUKEN_NAMESPACE