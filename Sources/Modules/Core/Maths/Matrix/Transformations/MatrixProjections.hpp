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
            scale / in_aspect, 0.0F ,  0.0F,                        0.0F,
            0.0F,              scale,  0.0F,                        0.0F,
            0.0F,              0.0F ,  in_far / (in_near - in_far), -(in_far * in_near) / (in_far - in_near),
            0.0F,              0.0F ,  1.0F,                        0.0F
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
        // https://www.kdab.com/projection-matrices-with-vulkan-part-1/
        // https://www.kdab.com/projection-matrices-with-vulkan-part-2/
        // Post view correction is used. This matrix is specific to vulkan.
        auto const top {-in_top};
        auto const bot {-in_bottom};

        auto const rl {in_right - in_left};
        auto const bt {bot      - top };
        auto const nf {in_near  - in_far };

        return Matrix<4, 4> {
            2.0_m / rl, 0.0F, 0.0F, 0.0F,
            0.0F, 2.0_m / bt, 0.0F, 0.0F,
            0.0F, 0.0F, 1.0_m / nf, 0.0F,

            -(in_right  + in_left) / rl,
            -(bot + top) / bt,
            in_near              / nf,
            1.0F
        };
    }
};

END_RUKEN_NAMESPACE