/*
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

#include "Types/Units/Pixels.hpp"

#include "Maths/Constants.hpp"
#include "Maths/Vector/Helper/VectorForward.hpp"

#include "Maths/Vector/Operations/VectorOperators.hpp"
#include "Maths/Vector/Operations/VectorMinMax.hpp"
#include "Maths/Vector/Operations/VectorLerp.hpp"

BEGIN_RUKEN_NAMESPACE

using Vector2px = Vector<2, Pixels>;

#pragma warning( push )
#pragma warning( disable:4201 )

/**
 * \brief Two dimensional pixel vector
 */
template <>
struct Vector<2, Pixels> final:
    VectorOperators<Vector<2, Pixels>>,
    VectorMinMax   <Vector<2, Pixels>>,
    VectorLerp     <Vector<2, Pixels>>
{
    #pragma region Members

    union
	{
	    Pixels data[2];

	    struct
	    {
	        Pixels x;
			Pixels y;
	    };
		struct
	    {
	        Pixels width;
			Pixels height;
	    };
	};

    #pragma endregion

    #pragma region Constructors

	constexpr Vector() noexcept:
	    data {0_px, 0_px}
	{}

    constexpr Vector(Pixels const in_width, Pixels const in_height) noexcept:
		data {in_width, in_height}
	{}

    #pragma endregion
};

#pragma warning( pop )

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