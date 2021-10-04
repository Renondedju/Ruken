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

template <FloatingPointType TType>
constexpr TType Lerp(TType in_source, TType in_destination, TType in_ratio) noexcept
{
    return (in_source + in_ratio * (in_destination - in_source));
}

template <FloatingPointType TType>
constexpr TType PingPong(TType in_value, TType in_range) noexcept
{
    return in_range - Abs<TType>(Repeat<TType>(in_value, static_cast<TType>(2) * in_range) - in_range);
}

template <FloatingPointType TType>
constexpr TType Repeat(TType in_value, TType in_range) noexcept
{
    return Clamp<TType>(in_value - Floor<TType>(in_value / in_range) * in_range, 0.0, in_range);
}

template <FloatingPointType TType>
TType QuadraticEaseIn(TType in_p) noexcept
{
    return in_p * in_p;
}

template <FloatingPointType TType>
TType QuadraticEaseOut(TType in_p) noexcept
{
    return -(in_p * (in_p - 2.0f));
}

template <FloatingPointType TType>
TType QuadraticEaseInOut(TType in_p) noexcept
{
    if(in_p < 0.5f)
        return 2.0f * in_p * in_p;

    return (-2.0f * in_p * in_p) + (4.0f * in_p) - 1.0f;
}


template <FloatingPointType TType>
TType CubicEaseIn(TType in_p) noexcept
{
    return in_p * in_p * in_p;
}

template <FloatingPointType TType>
TType CubicEaseOut(TType in_p) noexcept
{
    TType f = (in_p - 1.0f);
    return f * f * f + 1.0f;
}

template <FloatingPointType TType>
TType CubicEaseInOut(TType in_p) noexcept
{
    if(in_p < 0.5f)
        return 4.0f * in_p * in_p * in_p;

    TType f = ((2.0f * in_p) - 2.0f);
    return 0.5f * f * f * f + 1.0f;
}


template <FloatingPointType TType>
TType QuarticEaseIn(TType in_p) noexcept
{
    return in_p * in_p * in_p * in_p;
}

template <FloatingPointType TType>
TType QuarticEaseOut(TType in_p) noexcept
{
    TType f = (in_p - 1.0f);
    return f * f * f * (1.0f - in_p) + 1.0f;
}

template <FloatingPointType TType>
TType QuarticEaseInOut(TType in_p) noexcept
{
    if(in_p < 0.5f)
        return 8.0f * in_p * in_p * in_p * in_p;

    TType f = (in_p - 1.0f);
    return -8.0f * f * f * f * f + 1.0f;
}


template <FloatingPointType TType>
TType QuinticEaseIn(TType in_p) noexcept
{
    return in_p * in_p * in_p * in_p * in_p;
}

template <FloatingPointType TType>
TType QuinticEaseOut(TType in_p) noexcept
{
    TType f = (in_p - 1.0f);
    return f * f * f * f * f + 1.0f;
}

template <FloatingPointType TType>
TType QuinticEaseInOut(TType in_p) noexcept
{
    if(in_p < 0.5f)
        return 16.0f * in_p * in_p * in_p * in_p * in_p;

    TType f = ((2.0f * in_p) - 2.0f);
    return  0.5f * f * f * f * f * f + 1.0f;
}


template <FloatingPointType TType>
TType SineEaseIn(TType in_p) noexcept
{
    return Sin<TType>((in_p - 1.0f) * g_pi_2) + 1.0f;
}

template <FloatingPointType TType>
TType SineEaseOut(TType in_p) noexcept
{
    return Sin<TType>(in_p * g_pi_2);
}

template <FloatingPointType TType>
TType SineEaseInOut(TType in_p) noexcept
{
    return 0.5f * (1.0f - Cos<TType>(in_p * g_pi));
}


template <FloatingPointType TType>
TType CircularEaseIn(TType in_p) noexcept
{
    return 1.0f - Sqrt<TType>(1.0f - (in_p * in_p));
}

template <FloatingPointType TType>
TType CircularEaseOut(TType in_p) noexcept
{
    return Sqrt<TType>((2.0f - in_p) * in_p);
}

template <FloatingPointType TType>
TType CircularEaseInOut(TType in_p) noexcept
{
    if(in_p < 0.5f)
        return 0.5f * (1.0f - Sqrt<TType>(1.0f - 4.0f * (in_p * in_p)));

    return 0.5f * (Sqrt<TType>(-((2.0f * in_p) - 3.0f) * ((2.0f * in_p) - 1.0f)) + 1.0f);
}


template <FloatingPointType TType>
TType ExponentialEaseIn(TType in_p) noexcept
{
    return (in_p == 0.0f) ? in_p : Pow<TType>(2.0f, 10.0f * (in_p - 1.0f));
}

template <FloatingPointType TType>
TType ExponentialEaseOut(TType in_p) noexcept
{
    return (in_p == 1.0f) ? in_p : 1.0f - Pow<TType>(2.0f, -10.0f * in_p);
}

template <FloatingPointType TType>
TType ExponentialEaseInOut(TType in_p) noexcept
{
    if(in_p == 0.0f || in_p == 1.0f) return in_p;
    
    if(in_p < 0.5f)
        return 0.5f * Pow<TType>(2.0f, (20.0f * in_p) - 10.0f);

    return -0.5f * Pow<TType>(2.0f, (-20.0f * in_p) + 10.0f) + 1.0f;
}


template <FloatingPointType TType>
TType ElasticEaseIn(TType in_p) noexcept
{
    return Sin<TType>(13.0f * g_pi_2 * in_p) * Pow<TType>(2.0f, 10.0f * (in_p - 1.0f));
}

template <FloatingPointType TType>
TType ElasticEaseOut(TType in_p) noexcept
{
    return Sin<TType>(-13.0f * g_pi_2 * (in_p + 1.0f)) * Pow<TType>(2.0f, -10.0f * in_p) + 1.0f;
}

template <FloatingPointType TType>
TType ElasticEaseInOut(TType in_p) noexcept
{
    if(in_p < 0.5f)
        return 0.5f * Sin<TType>(13.0f * g_pi_2 * (2.0f * in_p)) * Pow<TType>(2.0f, 10.0f * ((2.0f * in_p) - 1.0f));

    return 0.5f * (Sin<TType>(-13.0f * g_pi_2 * ((2.0f * in_p - 1.0f) + 1.0f)) * Pow<TType>(2.0f, -10.0f * (2.0f * in_p - 1.0f)) + 2.0f);
}


template <FloatingPointType TType>
TType BackEaseIn(TType in_p) noexcept
{
    return in_p * in_p * in_p - in_p * Sin<TType>(in_p * g_pi);
}

template <FloatingPointType TType>
TType BackEaseOut(TType in_p) noexcept
{
    TType f = (1.0f - in_p);
    return 1.0f - (f * f * f - f * Sin<TType>(f * g_pi));
}

template <FloatingPointType TType>
TType BackEaseInOut(TType in_p) noexcept
{
    if(in_p < 0.5f)
    {
        TType f = 2.0f * in_p;
        return 0.5f * (f * f * f - f * Sin<TType>(f * g_pi));
    }

    TType f = (1.0f - (2.0f*in_p - 1.0f));
    return 0.5f * (1.0f - (f * f * f - f * Sin<TType>(f * g_pi))) + 0.5f;
}


template <FloatingPointType TType>
TType BounceEaseIn(TType in_p) noexcept
{
    return 1.0f - BounceEaseOut<TType>(1.0f - in_p);
}

template <FloatingPointType TType>
TType BounceEaseOut(TType in_p) noexcept
{
    if(in_p < 4.0f/11.0f)
        return (121.0f * in_p * in_p)/16.0f;

    if(in_p < 8.0f/11.0f)
        return (363.0f/40.0f * in_p * in_p) - (99.0f/10.0f * in_p) + 17.0f/5.0f;

    if(in_p < 9.0f/10.0f)
        return (4356.0f/361.0f * in_p * in_p) - (35442.0f/1805.0f * in_p) + 16061.0f/1805.0f;

    return (54.0f/5.0f * in_p * in_p) - (513.0f/25.0f * in_p) + 268.0f/25.0f;
}

template <FloatingPointType TType>
TType BounceEaseInOut(TType in_p) noexcept
{
    if(in_p < 0.5f)
        return 0.5f * BounceEaseIn<TType>(in_p*2.0f);

    return 0.5f * BounceEaseOut<TType>(in_p * 2.0f - 1.0f) + 0.5f;
}