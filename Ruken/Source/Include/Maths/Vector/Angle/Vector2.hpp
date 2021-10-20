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

#include "Types/Units/Angle/Angle.hpp"
#include "Types/Units/Angle/EAngleUnit.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma warning(push)
#pragma warning(disable: 4201) // warning C4201: nonstandard extension used : nameless struct/union

/**
 * \brief Two dimensional angle vector layout 
 * \tparam TAngleUnit Angle unit of the vector
 */
template <EAngleUnit TAngleUnit>
struct VectorLayout<2, Angle<TAngleUnit>>
{
    union
	{
	    Angle<TAngleUnit> data[2] {};
	    struct
	    {
	        Angle<TAngleUnit> x;
			Angle<TAngleUnit> y;
	    };
		struct
	    {
	        Angle<TAngleUnit> pitch;
			Angle<TAngleUnit> yaw;
	    };
	};
};

#pragma warning(pop)

/**
 * \brief Two dimensional distance vector
 * \tparam TAngleUnit Distance unit of the vector
 */
template <EAngleUnit TAngleUnit>
struct Vector<2, Angle<TAngleUnit>> : BaseVector<2, Angle<TAngleUnit>>
{
    using BaseVector<2, Angle<TAngleUnit>>::BaseVector;
};

using Vector2rad = Vector<2, Radians>;
using Vector2deg = Vector<2, Degrees>;

END_RUKEN_NAMESPACE