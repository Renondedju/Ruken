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

#include <iostream>

#include "Build/Namespace.hpp"

#include "Maths/Vector/AngleVector2.hpp"
#include "Maths/Vector/DistanceVector2.hpp"
#include "Maths/Vector/DistanceVector3.hpp"
#include "Maths/Vector/PixelVector2.hpp"
#include "Maths/Vector/PixelVector3.hpp"

USING_RUKEN_NAMESPACE

int main()
{
    Vector3m const distance {3_cm, 3_m, 3_km};
    Vector3m const result   {distance * Constants<Vector3cm>::forward};

    Vector3cm const test  = Constants<Vector2km>::up;
    Vector2cm const test2 = test * 2;

    Vector2rad angle = Vector2deg{};

    Vector2px resolution  {Vector3px{10_px, 10_px, 10_px}};
    Vector3cm resolution2 {Vector2cm{10_m, 10_m}};
    Vector3cm centimeters {Constants<Vector3km>::up * 2};

    Vector2km map_bounds {2.54_km, 2.587_km};
    map_bounds.Normalize();

    return EXIT_SUCCESS;
}
