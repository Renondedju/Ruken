#pragma once

#include "Core/Build/Namespace.hpp"
#include "Core/Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

struct Range
{
    #pragma region Members

    RkSize begin;
    RkSize size;

    #pragma endregion

    #pragma region Constructors

    /**
     * \brief Default constructor
     * \param in_begin Start index of the free range
     * \param in_size Size of the free range
     */
    Range(RkSize in_begin, RkSize in_size);
	Range			(Range const&) = default;
	Range			(Range&&     ) = default;
	Range& operator=(Range const&) = default;
	Range& operator=(Range&&     ) = default;
    ~Range()                       = default;

    #pragma endregion

    #pragma region Methods

    /**
     * \brief Reduces the range by one from the right
     * \return New size of the range
     */
    RkSize ReduceRight() noexcept;

    /**
     * \brief Expands the range to the left (moving the begin back by one)
     * \return New size of the range
     * \note This method will have no effect if the beginning of the range is already 0
     */
    RkSize ExpandLeft() noexcept;

    /**
     * \brief Expands the range to the right (increasing the size by one)
     * \return New size of the range
     */
    RkSize ExpandRight() noexcept;

    /**
     * \brief Checks if the range contains a given position
     * \param in_position Position to check
     * \return True if the range contains the passed position, false otherwise
     */
    [[nodiscard]]
    RkBool Contains(RkSize in_position) const noexcept;

    #pragma endregion
};

END_RUKEN_NAMESPACE