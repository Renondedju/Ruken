#pragma once

#include "Core/Types/Units/Pixels.hpp"
#include "Core/Build/Attributes.hpp"

#include "Core/Maths/Vector/Operations/VectorOperators.hpp"
#include "Core/Maths/Vector/Operations/VectorMinMax.hpp"
#include "Core/Maths/Vector/Operations/VectorLerp.hpp"
#include "Core/Maths/Vector/Helper/VectorForward.hpp"
#include "Core/Maths/Constants.hpp"

BEGIN_RUKEN_NAMESPACE

#ifdef RUKEN_COMPILER_MSVC
	#pragma warning( push )
	#pragma warning( disable:4201 )
#endif

/// @brief Two-dimensional pixel vector
template <>
struct RUKEN_EMPTY_BASES Vector<2, Pixels> final:
    VectorOperators<2, Pixels>,
    VectorMinMax   <2, Pixels>,
    VectorLerp     <2, Pixels>
{
	Pixels data[2];

	#pragma region Getters/Setters

	Pixels&		  x()       noexcept { return data[0]; }
	Pixels const& x() const noexcept { return data[0]; }

	Pixels&		  y()       noexcept { return data[1]; }
	Pixels const& y() const noexcept { return data[1]; }

	Pixels&		  Width ()       noexcept { return data[0]; }
	Pixels const& Width () const noexcept { return data[0]; }

	Pixels&		  Height()       noexcept { return data[1]; }
	Pixels const& Height() const noexcept { return data[1]; }

	#pragma endregion

    #pragma region Constructors

	constexpr Vector() noexcept:
	    data {0_px, 0_px}
	{}

    constexpr Vector(Pixels const in_width, Pixels const in_height) noexcept:
		data {in_width, in_height}
	{}

    #pragma endregion

    #pragma region Methods

    /**
	 * \brief Returns the aspect ratio of the vector (width/height)
	 * \return Aspect ratio
	 */
	constexpr RkFloat AspectRatio() const noexcept
	{
	    return static_cast<RkFloat>(Width()) / static_cast<RkFloat>(Height());
	}

    #pragma endregion
};

/// @brief Three-dimensional pixel vector
template <>
struct RUKEN_EMPTY_BASES Vector<3, Pixels> final:
	VectorOperators<3, Pixels>,
	VectorMinMax   <3, Pixels>,
	VectorLerp     <3, Pixels>
{
	Pixels data[3];

	#pragma region Getters/Setters

	Pixels&	      x()       noexcept { return data[0]; }
	Pixels const& x() const noexcept { return data[0]; }

	Pixels&	      y()       noexcept { return data[1]; }
	Pixels const& y() const noexcept { return data[1]; }

	Pixels&	      z()       noexcept { return data[2]; }
	Pixels const& z() const noexcept { return data[2]; }

	Pixels&	      Width ()       noexcept { return data[0]; }
	Pixels const& Width () const noexcept { return data[0]; }

	Pixels&	      Height()       noexcept { return data[1]; }
	Pixels const& Height() const noexcept { return data[1]; }

	Pixels&	      Depth ()       noexcept { return data[2]; }
	Pixels const& Depth () const noexcept { return data[2]; }

	#pragma endregion

	#pragma region Constructors

	constexpr Vector() noexcept:
		data {0_px, 0_px, 0_px}
	{}

	constexpr Vector(Pixels const in_width, Pixels const in_height, Pixels const in_depth) noexcept:
		data {in_width, in_height, in_depth}
	{}

	#pragma endregion
};

#ifdef RUKEN_COMPILER_MSVC
	#pragma warning( pop )
#endif

using Vector2px = Vector<2, Pixels>;
using Vector3px = Vector<3, Pixels>;

template <>
struct Constants<Vector2px>
{
    // Desktop definitions
    static constexpr Vector2px standard_definition {640_px , 480_px }; // SD (Standard Definition) - 480p
    static constexpr Vector2px high_definition     {1280_px, 720_px }; // HD (High Definition) - 720p
	static constexpr Vector2px full_hd             {1920_px, 1080_px}; // Full HD (FHD) - 1080p
	static constexpr Vector2px quad_hd             {2560_px, 1440_px}; // QHD (Quad HD) - 1440p
	static constexpr Vector2px ultra_hd            {3840_px, 2160_px}; // Ultra HD (UHD) - 4K / 2160p
	static constexpr Vector2px full_ultra_hd       {7680_px, 4320_px}; // Full Ultra HD - 8K / 4320p

	// Mobile definitions
	static constexpr Vector2px iphone_se {750_px , 1334_px};
	static constexpr Vector2px iphone_8  {750_px , 1334_px};
	static constexpr Vector2px iphone_x  {1125_px, 2436_px};
	static constexpr Vector2px iphone_12 {1170_px, 2532_px};

	static constexpr Vector2px galaxy_s7 {1440_px, 2560_px};
	static constexpr Vector2px galaxy_s8 {1440_px, 2960_px};
};

END_RUKEN_NAMESPACE