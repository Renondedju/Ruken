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
#include "Maths/Matrix/Matrix.hpp"
#include "Maths/Quaternion/Quaternion.hpp"
#include "Maths/Vector/DistanceVector3.hpp"
#include "Maths/Vector/PixelVector2.hpp"

USING_RUKEN_NAMESPACE

int main()
{
    Matrix const model {Matrix<4, 4>::ModelMatrix(
        Constants<Vector3m>  ::zero,      // Position
        Constants<Quaternion>::identity, // Rotation
        Constants<Vector3m>  ::one      // Scale
    )};

    Matrix const view {Matrix<4, 4>::LookAtMatrix(
        Constants<Vector3m>::zero,     // Position
        Constants<Vector3m>::forward, // Direction
        Constants<Vector3m>::up      // Up vector
    )};

    Matrix const projection {Matrix<4, 4>::PerspectiveProjectionMatrix(
        90_deg, Constants<Vector2px>::full_hd.AspectRatio(), 1_cm, 10_km
    )};

    Matrix const clip {Matrix<4, 4>::ClipSpace()};

    Matrix test {Matrix<4, 4>::ClipSpace()};
    test *= projection;

    std::cout << clip * projection * view * model << std::endl;

    return EXIT_SUCCESS;
}