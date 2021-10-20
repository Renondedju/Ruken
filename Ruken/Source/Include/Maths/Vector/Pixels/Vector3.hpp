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

#include "Types/Units/Pixels.hpp"
#include "Maths/Vector/BaseVector.hpp"
#include "Maths/Vector/VectorLayout.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma warning(push)
#pragma warning(disable: 4201) // warning C4201: nonstandard extension used : nameless struct/union

/**
 * \brief Three dimensional pixel vector layout 
 */
template <>
struct VectorLayout<3, Pixels>
{
    union
	{
	    Pixels data[3] {};
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
};

#pragma warning(pop)

/**
 * \brief Three dimensional pixel vector
 */
template <>
struct Vector<3, Pixels> : BaseVector<3, Pixels>
{
	using BaseVector<3, Pixels>::BaseVector;
};

using Vector3px = Vector<3, Pixels>;

END_RUKEN_NAMESPACE