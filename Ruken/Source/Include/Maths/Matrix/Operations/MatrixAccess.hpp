#pragma once

#include "Maths/Matrix/MatrixForward.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Implements matrix access methods
 */
struct MatrixAccess
{
    #pragma region Static Methods

    /**
     * \brief Getter
     * \param in_matrix Matrix instance
     * \param in_row Row index [0 to TRows)
     * \param in_column Column index [0 to TColumns)
     * \tparam TRows Height or number of rows of the matrix
     * \tparam TColumns Width or number of columns of the matrix
     * \return Constant reference tp the value at the selected position
     */
    template <RkSize TRows, RkSize TColumns>
    [[nodiscard]]
    static constexpr RkFloat const& At(Matrix<TRows, TColumns> const& in_matrix, RkSize const in_row, RkSize const in_column) noexcept
    {
        return in_matrix.data[in_row + in_column * TRows];
    }

    /**
     * \brief Getter/Setter
     * \param in_matrix Matrix instance
     * \param in_row Row index [0 to TRows)
     * \param in_column Column index [0 to TColumns)
     * \tparam TRows Height or number of rows of the matrix
     * \tparam TColumns Width or number of columns of the matrix
     * \return Reference to the value at the selected position
     */
    template <RkSize TRows, RkSize TColumns>
    [[nodiscard]]
    static constexpr RkFloat& At(Matrix<TRows, TColumns>& in_matrix, RkSize const in_row, RkSize const in_column) noexcept
    {
        return in_matrix.data[in_row + in_column * TRows];
    }

    #pragma endregion

    /**
     * \brief Implements matrix member access methods
     */
    template <RkSize TRows, RkSize TColumns>
    struct Member
    {
        using TMatrix = Matrix<TRows, TColumns>;

        #pragma region Methods

        /**
         * \brief Getter
         * \param in_row Row index [0 to TRows)
         * \param in_column Column index [0 to TColumns)
         * \return Constant reference tp the value at the selected position
         */
        [[nodiscard]]
        constexpr RkFloat const& At(RkSize const in_row, RkSize const in_column) const noexcept
        {
            return MatrixAccess::At(*static_cast<TMatrix const*>(this), in_row, in_column);
        }

        /**
         * \brief Getter/Setter
         * \param in_row Row index [0 to TRows)
         * \param in_column Column index [0 to TColumns)
         * \return Reference to the value at the selected position
         */
        [[nodiscard]]
        constexpr RkFloat& At(RkSize const in_row, RkSize const in_column) noexcept
        {
            return MatrixAccess::At(*static_cast<TMatrix*>(this), in_row, in_column);
        }

        #pragma endregion
    };
};

END_RUKEN_NAMESPACE