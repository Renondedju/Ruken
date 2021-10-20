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

#include "Maths/Vector/BaseVector.hpp"
#include "Maths/Vector/VectorLayout.hpp"

#include "Types/Units/Distance/Distance.hpp"
#include "Types/Units/Distance/EDistanceUnit.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma warning(push)
#pragma warning(disable: 4201) // warning C4201: nonstandard extension used : nameless struct/union

/**
 * \brief Two dimensional distance vector layout 
 * \tparam TDistanceUnit Distance unit of the vector
 */
template <EDistanceUnit TDistanceUnit>
struct VectorLayout<2, Distance<TDistanceUnit>>
{
    union
	{
	    Distance<TDistanceUnit> data[2] {};
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
};

#pragma warning(pop)

/**
 * \brief Two dimensional distance vector
 * \tparam TDistanceUnit Distance unit of the vector
 */
template <EDistanceUnit TDistanceUnit>
struct Vector<2, Distance<TDistanceUnit>> : BaseVector<2, Distance<TDistanceUnit>>
{
	using BaseVector<2, Distance<TDistanceUnit>>::BaseVector;

	static constexpr Vector right {Distance<TDistanceUnit>( 1.0F), Distance<TDistanceUnit>( 0.0F)};
	static constexpr Vector left  {Distance<TDistanceUnit>(-1.0F), Distance<TDistanceUnit>( 0.0F)};
	static constexpr Vector up    {Distance<TDistanceUnit>( 0.0F), Distance<TDistanceUnit>( 1.0F)};
	static constexpr Vector down  {Distance<TDistanceUnit>( 0.0F), Distance<TDistanceUnit>(-1.0F)};
};

using Vector2cm = Vector<2, Centimeters>;
using Vector2m  = Vector<2, Meters>;
using Vector2km = Vector<2, Kilometers>;

END_RUKEN_NAMESPACE