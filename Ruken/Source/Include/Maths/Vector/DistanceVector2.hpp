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

#include "Types/Units/Distance/Distance.hpp"
#include "Types/Units/Distance/EDistanceUnit.hpp"

#include "Maths/Vector/Helper/VectorForward.hpp"

#include "Maths/Vector/Operations/VectorNormalization.hpp"
#include "Maths/Vector/Operations/VectorOperators.hpp"
#include "Maths/Vector/Operations/VectorLength.hpp"
#include "Maths/Vector/Operations/VectorMinMax.hpp"
#include "Maths/Vector/Operations/VectorCross.hpp"
#include "Maths/Vector/Operations/VectorSlerp.hpp"
#include "Maths/Vector/Operations/VectorLerp.hpp"
#include "Maths/Vector/Operations/VectorDot.hpp"

BEGIN_RUKEN_NAMESPACE

using Vector2cm = Vector<2, Centimeters>;
using Vector2m  = Vector<2, Meters>;
using Vector2km = Vector<2, Kilometers>;

/**
 * \brief Two dimensional distance vector
 */
template <EDistanceUnit TDistanceUnit>
struct Vector<2, Distance<TDistanceUnit>> final:
    VectorNormalization<Vector<2, Distance<TDistanceUnit>>>,
    VectorOperators    <Vector<2, Distance<TDistanceUnit>>>,
    VectorMinMax       <Vector<2, Distance<TDistanceUnit>>>,
    VectorLength       <Vector<2, Distance<TDistanceUnit>>>,
    VectorCross        <Vector<2, Distance<TDistanceUnit>>>,
    VectorSlerp        <Vector<2, Distance<TDistanceUnit>>>,
    VectorLerp         <Vector<2, Distance<TDistanceUnit>>>,
    VectorDot          <Vector<2, Distance<TDistanceUnit>>>
{
    #pragma region Members

    union
	{
	    Distance<TDistanceUnit> data[2];

	    struct
	    {
	        Distance<TDistanceUnit> x;
			Distance<TDistanceUnit> y;
	    };
		struct
	    {
	        Distance<TDistanceUnit> width;
			Distance<TDistanceUnit> height;
	    };
	};

    #pragma endregion

    #pragma region Constructors

	constexpr Vector() noexcept:
	    data {Distance<TDistanceUnit>(0),
	          Distance<TDistanceUnit>(0)}
	{}

    constexpr Vector(Distance<TDistanceUnit> const in_x,
                     Distance<TDistanceUnit> const in_y) noexcept:
		data {in_x, in_y}
	{}

    #pragma endregion
};

template <EDistanceUnit TDistanceUnit>
struct Constants<Vector<2, Distance<TDistanceUnit>>>
{
	static constexpr Vector<2, Distance<TDistanceUnit>> right {
		Distance<TDistanceUnit>(1.0F),
        Distance<TDistanceUnit>(0.0F)
	};

	static constexpr Vector<2, Distance<TDistanceUnit>> left {
		Distance<TDistanceUnit>(-1.0F),
        Distance<TDistanceUnit>( 0.0F)
	};

	static constexpr Vector<2, Distance<TDistanceUnit>> up {
        Distance<TDistanceUnit>(0.0F),
		Distance<TDistanceUnit>(1.0F)
	};

	static constexpr Vector<2, Distance<TDistanceUnit>> down {
        Distance<TDistanceUnit>( 0.0F),
		Distance<TDistanceUnit>(-1.0F)
	};

};

END_RUKEN_NAMESPACE