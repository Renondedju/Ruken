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
 * \brief Two dimensional pixel vector layout 
 */
template <>
struct VectorLayout<2, Pixels>
{
    union
	{
	    Pixels data[2] {};
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
};

#pragma warning(pop)

/**
 * \brief Two dimensional pixel vector
 */
template <>
struct Vector<2, Pixels> : BaseVector<2, Pixels>
{
	using BaseVector<2, Pixels>::BaseVector;

	// Desktop definitions

    static const Vector standard_definition; // SD (Standard Definition) - 480p
    static const Vector high_definition;    // HD (High Definition) - 720p
	static const Vector full_hd;           // Full HD (FHD) - 1080p
	static const Vector quad_hd;          // QHD (Quad HD) - 1440p
	static const Vector ultra_hd;        // Ultra HD (UHD) - 4K / 2160p
	static const Vector full_ultra_hd;  // Full Ultra HD - 8K / 4320p

	// Mobile definitions

	static const Vector iphone_se;
	static const Vector iphone_8;
	static const Vector iphone_x;
	static const Vector iphone_12;

    static const Vector galaxy_s7;
	static const Vector galaxy_s8;
};

const Vector<2, Pixels> Vector<2, Pixels>::standard_definition {640_px , 480_px};
const Vector<2, Pixels> Vector<2, Pixels>::high_definition     {1280_px, 720_px};
const Vector<2, Pixels> Vector<2, Pixels>::full_hd             {1920_px, 1080_px};
const Vector<2, Pixels> Vector<2, Pixels>::quad_hd             {2560_px, 1440_px};
const Vector<2, Pixels> Vector<2, Pixels>::ultra_hd            {3840_px, 2160_px};
const Vector<2, Pixels> Vector<2, Pixels>::full_ultra_hd       {7680_px, 4320_px};
const Vector<2, Pixels> Vector<2, Pixels>::iphone_se           {750_px , 1334_px};
const Vector<2, Pixels> Vector<2, Pixels>::iphone_8            {750_px , 1334_px};
const Vector<2, Pixels> Vector<2, Pixels>::iphone_x            {1125_px, 2436_px};
const Vector<2, Pixels> Vector<2, Pixels>::iphone_12           {1170_px, 2532_px};
const Vector<2, Pixels> Vector<2, Pixels>::galaxy_s7           {1440_px, 2560_px};
const Vector<2, Pixels> Vector<2, Pixels>::galaxy_s8           {1440_px, 2960_px};

using Vector2px = Vector<2, Pixels>;

END_RUKEN_NAMESPACE