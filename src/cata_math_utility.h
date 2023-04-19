#pragma once
#ifndef CATA_SRC_CATA_MATH_UTILITY_H
#define CATA_SRC_CATA_MATH_UTILITY_H

#include <algorithm>
#include <utility>
#include <vector>

/**
 * Clamp (number and space wise) value to with,
 * taking into account the specified preferred scale,
 * returning the adjusted (shortened) scale that best fit the width,
 * optionally returning a flag that indicate if the value was truncated to fit the width
 */
/**@{*/
double clamp_to_width( double value, int width, int &scale );
double clamp_to_width( double value, int width, int &scale, bool *out_truncated );
/**@}*/

/**
 * Clamp first argument so that it is no lower than second and no higher than third.
 * Does not check if min is lower than max.
 */
template<typename T>
constexpr T clamp( const T &val, const T &min, const T &max )
{
    return std::max( min, std::min( max, val ) );
}

/**
 * Linear interpolation: returns first argument if t is 0, second if t is 1, otherwise proportional to t.
 * Does not clamp t, meaning it can return values lower than min (if t<0) or higher than max (if t>1).
 */
template<typename T>
constexpr T lerp( const T &min, const T &max, float t )
{
    return ( 1.0f - t ) * min + t * max;
}

/** Linear interpolation with t clamped to [0, 1] */
template<typename T>
constexpr T lerp_clamped( const T &min, const T &max, float t )
{
    return lerp( min, max, clamp( t, 0.0f, 1.0f ) );
}

/**
 * From `points`, finds p1 and p2 such that p1.first < x < p2.first
 * Then linearly interpolates between p1.second and p2.second and returns the result.
 * `points` should be sorted by first elements of the pairs.
 * If x is outside range, returns second value of the first (if x < points[0].first) or last point.
 */
float multi_lerp( const std::vector<std::pair<float, float>> &points, float x );

int modulo( int v, int m );

#endif // CATA_SRC_CATA_MATH_UTILITY_H
