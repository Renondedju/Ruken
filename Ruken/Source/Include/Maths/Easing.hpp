
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

// Interpolations 

/**
 * \brief Computes a linear interpolation between two value
 *
 * \param in_source Source number
 * \param in_destination Destination number
 * \param in_ratio Interpolation ratio
 *
 * \return Interpolated value
 */
[[nodiscard]] RkFloat Lerp(RkFloat in_source, RkFloat in_destination, RkFloat in_ratio) noexcept;

/**
 * \brief PingPongs in_value, so that it is never larger than in_range and never smaller than 0.0.
 *
 * The returned value will move back and forth between 0 and in_range.
 *
 * \param in_value Input value
 * \param in_range Range of the ping pong
 *
 * \return Transformed value
 */
[[nodiscard]] RkFloat PingPong(RkFloat in_value, RkFloat in_range) noexcept;

/**
 * \brief Loops a given value on a given range
 *
 * \param in_value Input value
 * \param in_range Range of the loop
 *
 * \return Looped value
 */
[[nodiscard]] RkFloat Repeat(RkFloat in_value, RkFloat in_range) noexcept;

// -> Quadratic easing: p^2

/**
 * \brief Modeled after the parabola y = x^2
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat QuadraticEaseIn(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the parabola y = -x^2 + 2x
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat QuadraticEaseOut(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the piecewise quadratic
 * y = (1/2)((2x)^2)             ; [0, 0.5)
 * y = -(1/2)((2x-1)*(2x-3) - 1) ; [0.5, 1]
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat QuadraticEaseInOut(RkFloat in_p) noexcept;

// -> Cubic easing: p^3

/**
 * \brief Modeled after the cubic y = x^3
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat CubicEaseIn(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the cubic y = (x - 1)^3 + 1
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat CubicEaseOut(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the piecewise cubic
 * y = (1/2)((2x)^3)       ; [0, 0.5)
 * y = (1/2)((2x-2)^3 + 2) ; [0.5, 1]
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat CubicEaseInOut(RkFloat in_p) noexcept;

// -> Quartic easing: p^4

/**
 * \brief Modeled after the quartic x^4
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat QuarticEaseIn(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the quartic y = 1 - (x - 1)^4
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat QuarticEaseOut(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the piecewise quartic
 * y = (1/2)((2x)^4)        ; [0, 0.5)
 * y = -(1/2)((2x-2)^4 - 2) ; [0.5, 1]
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat QuarticEaseInOut(RkFloat in_p) noexcept;

// -> Quintic easing: p^5

/**
 * \brief Modeled after the quintic y = x^5
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat QuinticEaseIn(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the quintic y = (x - 1)^5 + 1
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat QuinticEaseOut(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the piecewise quintic
 * y = (1/2)((2x)^5)       ; [0, 0.5)
 * y = (1/2)((2x-2)^5 + 2) ; [0.5, 1]
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat QuinticEaseInOut(RkFloat in_p) noexcept;

// -> Sine wave easing: sin(p * PI/2)

/**
 * \brief Modeled after quarter-cycle of sine wave
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat SineEaseIn(RkFloat in_p) noexcept;

/**
 * \brief Modeled after quarter-cycle of sine wave (different phase)
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat SineEaseOut(RkFloat in_p) noexcept;

/**
 * \brief Modeled after half sine wave
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat SineEaseInOut(RkFloat in_p) noexcept;

// -> Circular easing: sqrt(1 - p^2)

/**
 * \brief Modeled after shifted quadrant IV of unit circle
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat CircularEaseIn(RkFloat in_p) noexcept;

/**
 * \brief Modeled after shifted quadrant II of unit circle
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat CircularEaseOut(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the piecewise circular function
 * y = (1/2)(1 - sqrt(1 - 4x^2))           ; [0, 0.5)
 * y = (1/2)(sqrt(-(2x - 3)*(2x - 1)) + 1) ; [0.5, 1]
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat CircularEaseInOut(RkFloat in_p) noexcept;

// -> Exponential easing, base 2

/**
 * \brief Modeled after the exponential function y = 2^(10(x - 1))
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat ExponentialEaseIn(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the exponential function y = -2^(-10x) + 1
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat ExponentialEaseOut(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the piecewise exponential
 * y = (1/2)2^(10(2x - 1))         ; [0,0.5)
 * y = -(1/2)*2^(-10(2x - 1))) + 1 ; [0.5,1]
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat ExponentialEaseInOut(RkFloat in_p) noexcept;

// -> Exponentially-damped sine wave easing

/**
 * \brief Modeled after the damped sine wave y = sin(13pi/2*x)*pow(2, 10 * (x - 1))
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat ElasticEaseIn(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the damped sine wave y = sin(-13pi/2*(x + 1))*pow(2, -10x) + 1
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat ElasticEaseOut(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the piecewise exponentially-damped sine wave:
 * y = (1/2)*sin(13pi/2*(2*x))*pow(2, 10 * ((2*x) - 1))      ; [0,0.5)
 * y = (1/2)*(sin(-13pi/2*((2x-1)+1))*pow(2,-10(2*x-1)) + 2) ; [0.5, 1]
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat ElasticEaseInOut(RkFloat in_p) noexcept;

// -> Overshooting cubic easing

/**
 * \brief Modeled after the overshooting cubic y = x^3-x*sin(x*pi)
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat BackEaseIn(RkFloat in_p) noexcept;

/**
 * \brief Modeled after overshooting cubic y = 1-((1-x)^3-(1-x)*sin((1-x)*pi))
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat BackEaseOut(RkFloat in_p) noexcept;

/**
 * \brief Modeled after the piecewise overshooting cubic function:
 * y = (1/2)*((2x)^3-(2x)*sin(2*x*pi))           ; [0, 0.5)
 * y = (1/2)*(1-((1-x)^3-(1-x)*sin((1-x)*pi))+1) ; [0.5, 1]
 *
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat BackEaseInOut(RkFloat in_p) noexcept;

// -> Exponentially-decaying bounce easing

/**
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat BounceEaseIn(RkFloat in_p) noexcept;

/**
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat BounceEaseOut(RkFloat in_p) noexcept;

/**
 * \param in_p Value to ease, should be contained in the following range [0; 1]
 * \return Eased value, contained in the following range [0; 1]
 */
[[nodiscard]] RkFloat BounceEaseInOut(RkFloat in_p) noexcept;

END_RUKEN_NAMESPACE
