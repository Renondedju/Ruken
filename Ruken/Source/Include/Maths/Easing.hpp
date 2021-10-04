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

#include "Build/Namespace.hpp"
#include "Maths/Constants.hpp" // Used in the inline file
#include "Types/Concepts/FloatingPointType.hpp"

BEGIN_RUKEN_NAMESPACE

// Interpolations 

/**
 * \brief Computes a linear interpolation between two value
 *
 * \tparam TType Type to operate with
 *
 * \param in_source Source number
 * \param in_destination Destination number
 * \param in_ratio Interpolation ratio
 *
 * \return Interpolated value
 */
template <FloatingPointType TType>
[[nodiscard]] constexpr TType Lerp(TType in_source, TType in_destination, TType in_ratio) noexcept;

/**
 * \brief PingPongs in_value, so that it is never larger than in_range and never smaller than 0.0.
 *
 * The returned value will move back and forth between 0 and in_range.
 *
 * \tparam TType Type to operate with
 * \param in_value Input value
 * \param in_range Range of the ping pong
 *
 * \return Transformed value
 */
template <FloatingPointType TType>
[[nodiscard]] constexpr TType PingPong(TType in_value, TType in_range) noexcept;

/**
 * \brief Loops a given value on a given range
 *
 * \tparam TType Type to operate with
 * \param in_value Input value
 * \param in_range Range of the loop
 *
 * \return Looped value
 */
template <FloatingPointType TType>
[[nodiscard]] constexpr TType Repeat(TType in_value, TType in_range) noexcept;

// -> Quadratic easing: p^2

/**
 * \brief Modeled after the parabola y = x^2
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType QuadraticEaseIn(TType in_p) noexcept;

/**
 * \brief Modeled after the parabola y = -x^2 + 2x
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType QuadraticEaseOut(TType in_p) noexcept;

/**
 * \brief Modeled after the piecewise quadratic
 * y = (1/2)((2x)^2)             ; [0, 0.5)
 * y = -(1/2)((2x-1)*(2x-3) - 1) ; [0.5, 1]
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType QuadraticEaseInOut(TType in_p) noexcept;

// -> Cubic easing: p^3

/**
 * \brief Modeled after the cubic y = x^3
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType CubicEaseIn(TType in_p) noexcept;

/**
 * \brief Modeled after the cubic y = (x - 1)^3 + 1
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType CubicEaseOut(TType in_p) noexcept;

/**
 * \brief Modeled after the piecewise cubic
 * y = (1/2)((2x)^3)       ; [0, 0.5)
 * y = (1/2)((2x-2)^3 + 2) ; [0.5, 1]
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType CubicEaseInOut(TType in_p) noexcept;

// -> Quartic easing: p^4

/**
 * \brief Modeled after the quartic x^4
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType QuarticEaseIn(TType in_p) noexcept;

/**
 * \brief Modeled after the quartic y = 1 - (x - 1)^4
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType QuarticEaseOut(TType in_p) noexcept;

/**
 * \brief Modeled after the piecewise quartic
 * y = (1/2)((2x)^4)        ; [0, 0.5)
 * y = -(1/2)((2x-2)^4 - 2) ; [0.5, 1]
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType QuarticEaseInOut(TType in_p) noexcept;

// -> Quintic easing: p^5

/**
 * \brief Modeled after the quintic y = x^5
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType QuinticEaseIn(TType in_p) noexcept;

/**
 * \brief Modeled after the quintic y = (x - 1)^5 + 1
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType QuinticEaseOut(TType in_p) noexcept;

/**
 * \brief Modeled after the piecewise quintic
 * y = (1/2)((2x)^5)       ; [0, 0.5)
 * y = (1/2)((2x-2)^5 + 2) ; [0.5, 1]
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType QuinticEaseInOut(TType in_p) noexcept;

// -> Sine wave easing: sin(p * PI/2)

/**
 * \brief Modeled after quarter-cycle of sine wave
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType SineEaseIn(TType in_p) noexcept;

/**
 * \brief Modeled after quarter-cycle of sine wave (different phase)
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType SineEaseOut(TType in_p) noexcept;

/**
 * \brief Modeled after half sine wave
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType SineEaseInOut(TType in_p) noexcept;

// -> Circular easing: sqrt(1 - p^2)

/**
 * \brief Modeled after shifted quadrant IV of unit circle
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType CircularEaseIn(TType in_p) noexcept;

/**
 * \brief Modeled after shifted quadrant II of unit circle
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType CircularEaseOut(TType in_p) noexcept;

/**
 * \brief Modeled after the piecewise circular function
 * y = (1/2)(1 - sqrt(1 - 4x^2))           ; [0, 0.5)
 * y = (1/2)(sqrt(-(2x - 3)*(2x - 1)) + 1) ; [0.5, 1]
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType CircularEaseInOut(TType in_p) noexcept;

// -> Exponential easing, base 2

/**
 * \brief Modeled after the exponential function y = 2^(10(x - 1))
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType ExponentialEaseIn(TType in_p) noexcept;

/**
 * \brief Modeled after the exponential function y = -2^(-10x) + 1
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType ExponentialEaseOut(TType in_p) noexcept;

/**
 * \brief Modeled after the piecewise exponential
 * y = (1/2)2^(10(2x - 1))         ; [0,0.5)
 * y = -(1/2)*2^(-10(2x - 1))) + 1 ; [0.5,1]
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType ExponentialEaseInOut(TType in_p) noexcept;

// -> Exponentially-damped sine wave easing

/**
 * \brief Modeled after the damped sine wave y = sin(13pi/2*x)*pow(2, 10 * (x - 1))
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType ElasticEaseIn(TType in_p) noexcept;

/**
 * \brief Modeled after the damped sine wave y = sin(-13pi/2*(x + 1))*pow(2, -10x) + 1
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType ElasticEaseOut(TType in_p) noexcept;

/**
 * \brief Modeled after the piecewise exponentially-damped sine wave:
 * y = (1/2)*sin(13pi/2*(2*x))*pow(2, 10 * ((2*x) - 1))      ; [0,0.5)
 * y = (1/2)*(sin(-13pi/2*((2x-1)+1))*pow(2,-10(2*x-1)) + 2) ; [0.5, 1]
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType ElasticEaseInOut(TType in_p) noexcept;

// -> Overshooting cubic easing

/**
 * \brief Modeled after the overshooting cubic y = x^3-x*sin(x*pi)
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType BackEaseIn(TType in_p) noexcept;

/**
 * \brief Modeled after overshooting cubic y = 1-((1-x)^3-(1-x)*sin((1-x)*pi))
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType BackEaseOut(TType in_p) noexcept;

/**
 * \brief Modeled after the piecewise overshooting cubic function:
 * y = (1/2)*((2x)^3-(2x)*sin(2*x*pi))           ; [0, 0.5)
 * y = (1/2)*(1-((1-x)^3-(1-x)*sin((1-x)*pi))+1) ; [0.5, 1]
 *
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType BackEaseInOut(TType in_p) noexcept;

// -> Exponentially-decaying bounce easing

/**
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType BounceEaseIn(TType in_p) noexcept;

/**
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType BounceEaseOut(TType in_p) noexcept;

/**
 * \tparam TType Type to operate with
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
template <FloatingPointType TType>
[[nodiscard]] TType BounceEaseInOut(TType in_p) noexcept;

#include "Maths/Easing.inl"

END_RUKEN_NAMESPACE
