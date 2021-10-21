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

#include "Maths/Math.hpp"
#include "Maths/Easing.hpp"
#include "Maths/Utility.hpp"
#include "Maths/Constants.hpp"
#include "Maths/Trigonometry.hpp"

USING_RUKEN_NAMESPACE

RkFloat RUKEN_NAMESPACE::Lerp(RkFloat const in_source, RkFloat const in_destination, RkFloat const in_ratio) noexcept
{
    return in_source + in_ratio * (in_destination - in_source);
}

RkFloat RUKEN_NAMESPACE::PingPong(RkFloat const in_value, RkFloat const in_range) noexcept
{
    return in_range - Abs(Repeat(in_value, 2.0F * in_range) - in_range);
}

RkFloat RUKEN_NAMESPACE::Repeat(RkFloat const in_value, RkFloat const in_range) noexcept
{
    return Clamp(in_value - Floor(in_value / in_range) * in_range, 0.0F, in_range);
}

RkFloat RUKEN_NAMESPACE::QuadraticEaseIn(RkFloat const in_p) noexcept
{
    return in_p * in_p;
}

RkFloat RUKEN_NAMESPACE::QuadraticEaseOut(RkFloat const in_p) noexcept
{
    return -(in_p * (in_p - 2.0F));
}

RkFloat RUKEN_NAMESPACE::QuadraticEaseInOut(RkFloat const in_p) noexcept
{
    if (in_p < 0.5F)
        return 2.0F * in_p * in_p;

    return -2.0F * in_p * in_p + 4.0F * in_p - 1.0F;
}

RkFloat RUKEN_NAMESPACE::CubicEaseIn(RkFloat const in_p) noexcept
{
    return in_p * in_p * in_p;
}

RkFloat RUKEN_NAMESPACE::CubicEaseOut(RkFloat const in_p) noexcept
{
    RkFloat const f = in_p - 1.0F;
    return f * f * f + 1.0F;
}

RkFloat RUKEN_NAMESPACE::CubicEaseInOut(RkFloat const in_p) noexcept
{
    if (in_p < 0.5F)
        return 4.0F * in_p * in_p * in_p;

    RkFloat const f = 2.0F * in_p - 2.0F;
    return 0.5F * f * f * f + 1.0F;
}

RkFloat RUKEN_NAMESPACE::QuarticEaseIn(RkFloat const in_p) noexcept
{
    return in_p * in_p * in_p * in_p;
}

RkFloat RUKEN_NAMESPACE::QuarticEaseOut(RkFloat const in_p) noexcept
{
    RkFloat const f = in_p - 1.0F;
    return f * f * f * (1.0F - in_p) + 1.0F;
}

RkFloat RUKEN_NAMESPACE::QuarticEaseInOut(RkFloat const in_p) noexcept
{
    if (in_p < 0.5F)
        return 8.0F * in_p * in_p * in_p * in_p;

    RkFloat const f = in_p - 1.0F;
    return -8.0F * f * f * f * f + 1.0F;
}

RkFloat RUKEN_NAMESPACE::QuinticEaseIn(RkFloat const in_p) noexcept
{
    return in_p * in_p * in_p * in_p * in_p;
}

RkFloat RUKEN_NAMESPACE::QuinticEaseOut(RkFloat const in_p) noexcept
{
    RkFloat const f = in_p - 1.0F;
    return f * f * f * f * f + 1.0F;
}

RkFloat RUKEN_NAMESPACE::QuinticEaseInOut(RkFloat const in_p) noexcept
{
    if (in_p < 0.5F)
        return 16.0F * in_p * in_p * in_p * in_p * in_p;

    RkFloat const f = 2.0F * in_p - 2.0F;
    return 0.5F * f * f * f * f * f + 1.0F;
}

RkFloat RUKEN_NAMESPACE::SineEaseIn(RkFloat const in_p) noexcept
{
    return Sin(Radians(in_p - 1.0F * Constants<RkFloat>::pi_2)) + 1.0F;
}

RkFloat RUKEN_NAMESPACE::SineEaseOut(RkFloat const in_p) noexcept
{
    return Sin(Radians(in_p * Constants<RkFloat>::pi_2));
}

RkFloat RUKEN_NAMESPACE::SineEaseInOut(RkFloat const in_p) noexcept
{
    return 0.5F * (1.0F - Cos(Radians(in_p * Constants<RkFloat>::pi)));
}

RkFloat RUKEN_NAMESPACE::CircularEaseIn(RkFloat const in_p) noexcept
{
    return 1.0F - Sqrt(1.0F - in_p * in_p);
}

RkFloat RUKEN_NAMESPACE::CircularEaseOut(RkFloat const in_p) noexcept
{
    return Sqrt((2.0F - in_p) * in_p);
}

RkFloat RUKEN_NAMESPACE::CircularEaseInOut(RkFloat const in_p) noexcept
{
    if (in_p < 0.5F)
        return 0.5F * (1.0F - Sqrt(1.0F - 4.0F * (in_p * in_p)));

    return 0.5F * (Sqrt(-((2.0F * in_p) - 3.0F) * ((2.0F * in_p) - 1.0F)) + 1.0F);
}

RkFloat RUKEN_NAMESPACE::ExponentialEaseIn(RkFloat const in_p) noexcept
{
    return (in_p == 0.0F) ? in_p : Pow(2.0F, 10.0F * (in_p - 1.0F));
}

RkFloat RUKEN_NAMESPACE::ExponentialEaseOut(RkFloat const in_p) noexcept
{
    return (in_p == 1.0F) ? in_p : 1.0F - Pow(2.0F, -10.0F * in_p);
}

RkFloat RUKEN_NAMESPACE::ExponentialEaseInOut(RkFloat const in_p) noexcept
{
    if (in_p == 0.0F || in_p == 1.0F)
        return in_p;

    if (in_p < 0.5F)
        return 0.5F * Pow(2.0F, (20.0F * in_p) - 10.0F);

    return -0.5F * Pow(2.0F, (-20.0F * in_p) + 10.0F) + 1.0F;
}

RkFloat RUKEN_NAMESPACE::ElasticEaseIn(RkFloat const in_p) noexcept
{
    return Sin(Radians(13.0F * Constants<RkFloat>::pi_2 * in_p)) * Pow(2.0F, 10.0F * (in_p - 1.0F));
}

RkFloat RUKEN_NAMESPACE::ElasticEaseOut(RkFloat const in_p) noexcept
{
    return Sin(Radians(-13.0F * Constants<RkFloat>::pi_2 * (in_p + 1.0F))) * Pow(2.0F, -10.0F * in_p) + 1.0F;
}

RkFloat RUKEN_NAMESPACE::ElasticEaseInOut(RkFloat const in_p) noexcept
{
    if (in_p < 0.5F)
        return 0.5F * Sin(Radians(13.0F * Constants<RkFloat>::pi_2 * (2.0F * in_p))) * Pow(2.0F, 10.0F * ((2.0F * in_p) - 1.0F));

    return 0.5F * (Sin(Radians(-13.0F * Constants<RkFloat>::pi_2 * ((2.0F * in_p - 1.0F) + 1.0F))) * Pow(2.0F, -10.0F * (2.0F * in_p - 1.0F)) + 2.0F);
}

RkFloat RUKEN_NAMESPACE::BackEaseIn(RkFloat const in_p) noexcept
{
    return in_p * in_p * in_p - in_p * Sin(Radians(in_p * Constants<RkFloat>::pi));
}

RkFloat RUKEN_NAMESPACE::BackEaseOut(RkFloat const in_p) noexcept
{
    RkFloat f = (1.0F - in_p);
    return 1.0F - (f * f * f - f * Sin(Radians(f * Constants<RkFloat>::pi)));
}

RkFloat RUKEN_NAMESPACE::BackEaseInOut(RkFloat const in_p) noexcept
{
    if (in_p < 0.5F)
    {
        RkFloat f = 2.0F * in_p;
        return 0.5F * (f * f * f - f * Sin(Radians(f * Constants<RkFloat>::pi)));
    }

    RkFloat f = (1.0F - (2.0F * in_p - 1.0F));
    return 0.5F * (1.0F - (f * f * f - f * Sin(Radians(f * Constants<RkFloat>::pi)))) + 0.5F;
}

RkFloat RUKEN_NAMESPACE::BounceEaseIn(RkFloat const in_p) noexcept
{
    return 1.0F - BounceEaseOut(1.0F - in_p);
}

RkFloat RUKEN_NAMESPACE::BounceEaseOut(RkFloat const in_p) noexcept
{
    if (in_p < 4.0F / 11.0F)
        return (121.0F * in_p * in_p) / 16.0F;

    if (in_p < 8.0F / 11.0F)
        return (363.0F / 40.0F * in_p * in_p) - (99.0F / 10.0F * in_p) + 17.0F / 5.0F;

    if (in_p < 9.0F / 10.0F)
        return (4356.0F / 361.0F * in_p * in_p) - (35442.0F / 1805.0F * in_p) + 16061.0F / 1805.0F;

    return (54.0F / 5.0F * in_p * in_p) - (513.0F / 25.0F * in_p) + 268.0F / 25.0F;
}

RkFloat RUKEN_NAMESPACE::BounceEaseInOut(RkFloat const in_p) noexcept
{
    if (in_p < 0.5F)
        return 0.5F * BounceEaseIn(in_p * 2.0F);

    return 0.5F * BounceEaseOut(in_p * 2.0F - 1.0F) + 0.5F;
}
