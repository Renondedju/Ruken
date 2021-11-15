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

#include "Build/Attributes/EmptyBases.hpp"

#include "Types/Units/Pixels.hpp"
#include "Maths/Vector/Helper/VectorForward.hpp"
#include "Maths/Vector/Operations/VectorLerp.hpp"
#include "Maths/Vector/Operations/VectorMinMax.hpp"
#include "Maths/Vector/Operations/VectorOperators.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma warning( push )
#pragma warning( disable:4201 )

/**
 * \brief Three dimensional pixel vector
 */
template <>
struct RUKEN_EMPTY_BASES Vector<3, Pixels> final:
    VectorOperators<3, Pixels>,
    VectorMinMax   <3, Pixels>,
    VectorLerp     <3, Pixels>
{
    #pragma region Members

    union
	{
	    Pixels data[3];

	    struct
	    {
	        Pixels x;
			Pixels y;
			Pixels z;
	    };
		struct
	    {
	        Pixels width;
			Pixels height;
			Pixels depth;
	    };
	};

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

#pragma warning( pop )

using Vector3px = Vector<3, Pixels>;

END_RUKEN_NAMESPACE