#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <limits>

struct Math
{
    static bool IsZero( float value ) { return std::fabs( value ) < std::numeric_limits< float >::epsilon(); }
};

#endif  // MATH_HPP
