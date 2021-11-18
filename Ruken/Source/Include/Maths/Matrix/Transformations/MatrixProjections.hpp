#pragma once

#include "Maths/Trigonometry.hpp"

#include "Maths/Matrix/MatrixForward.hpp"

#include "Types/Units/Angle/Angle.hpp"
#include "Types/Units/Distance/Distance.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements 3D projection matrices (orthogonal & perspective)
 * \tparam TRows Number of rows of the matrix
 * \tparam TColumns Number of columns of the matrix
 * \tparam TSfinae Special parameter allowing selection of class specialization to enable or disable some functions 
 */
template <RkSize TRows, RkSize TColumns, typename TSfinae = RkVoid>
struct MatrixProjections
{};

// Requires a 4x4 matrix
template <RkSize TRows, RkSize TColumns>
struct MatrixProjections<TRows, TColumns, std::enable_if_t<TRows == 4 && TColumns == 4>>
{
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
    static Matrix<TRows, TColumns> PerspectiveProjectionMatrix(
        Radians const in_fov,
        RkFloat const in_aspect,
        Meters  const in_near,
        Meters  const in_far) noexcept
    {
        RkFloat const scale = 1.0F / Tan(in_fov / 2.0F);

        return Matrix<4, 4> {
            scale / in_aspect, 0.0F ,  0.0F,                                              0.0F,
            0.0F,              scale,  0.0F,                                              0.0F,
            0.0F,              0.0F ,  static_cast<RkFloat>(in_far / (in_near - in_far)), static_cast<RkFloat>(-(in_far * in_near) / (in_far - in_near)),
            0.0F,              0.0F , -1.0F,                                              0.0F
        };
    }

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
    static Matrix<TRows, TColumns> OrthogonalProjectionMatrix(
            Meters const in_left  , Meters const in_right, 
            Meters const in_bottom, Meters const in_top, 
            Meters const in_near  , Meters const in_far) noexcept
    {
        return Matrix<4, 4> {
            static_cast<RkFloat>(2.0F / (in_right - in_left)), 0.0F, 0.0F, 0.0F,
            0.0F, static_cast<RkFloat>(2.0F / (in_bottom - in_top)), 0.0F, 0.0F,
            0.0F, 0.0F, static_cast<RkFloat>(1.0F / (in_near - in_far)), 0.0F,

            static_cast<RkFloat>(-(in_right + in_left) / (in_right - in_left)),
            static_cast<RkFloat>(-(in_bottom + in_top) / (in_bottom - in_top)),
            static_cast<RkFloat>(in_near / (in_near - in_far)),
            1.0F
        };
    }
};

END_RUKEN_NAMESPACE