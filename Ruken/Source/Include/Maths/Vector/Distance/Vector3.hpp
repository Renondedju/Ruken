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
 * \brief Three dimensional distance vector layout 
 * \tparam TDistanceUnit Distance unit of the vector
 */
template <EDistanceUnit TDistanceUnit>
struct VectorLayout<3, Distance<TDistanceUnit>>
{
	union
	{
	    Distance<TDistanceUnit> data[3] {};
	    struct
	    {
	        Distance<TDistanceUnit> x;
			Distance<TDistanceUnit> y;
			Distance<TDistanceUnit> z;
	    };
		struct
	    {
	        Distance<TDistanceUnit> width;
			Distance<TDistanceUnit> height;
			Distance<TDistanceUnit> depth;
	    };
	};
};

#pragma warning(pop)

/**
 * \brief Three dimensional distance vector
 * \tparam TDistanceUnit Distance unit of the vector
 */
template <EDistanceUnit TDistanceUnit>
struct Vector<3, Distance<TDistanceUnit>> : BaseVector<3, Distance<TDistanceUnit>>
{
	using BaseVector<3, Distance<TDistanceUnit>>::BaseVector;

	static constexpr Vector right    = Vector(Distance<TDistanceUnit>( 1.0F), Distance<TDistanceUnit>( 0.0F), Distance<TDistanceUnit>( 0.0F));
	static constexpr Vector left     = Vector(Distance<TDistanceUnit>(-1.0F), Distance<TDistanceUnit>( 0.0F), Distance<TDistanceUnit>( 0.0F));
	static constexpr Vector up       = Vector(Distance<TDistanceUnit>( 0.0F), Distance<TDistanceUnit>( 1.0F), Distance<TDistanceUnit>( 0.0F));
	static constexpr Vector down     = Vector(Distance<TDistanceUnit>( 0.0F), Distance<TDistanceUnit>(-1.0F), Distance<TDistanceUnit>( 0.0F));
	static constexpr Vector forward  = Vector(Distance<TDistanceUnit>( 0.0F), Distance<TDistanceUnit>( 0.0F), Distance<TDistanceUnit>( 1.0F));
	static constexpr Vector backward = Vector(Distance<TDistanceUnit>( 0.0F), Distance<TDistanceUnit>( 0.0F), Distance<TDistanceUnit>(-1.0F));
};

using Vector3cm = Vector<3, Centimeters>;
using Vector3m  = Vector<3, Meters>;
using Vector3km = Vector<3, Kilometers>;

END_RUKEN_NAMESPACE