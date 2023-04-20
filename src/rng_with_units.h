#pragma once
#ifndef CATA_SRC_RNG_WITH_UNITS_H
#define CATA_SRC_RNG_WITH_UNITS_H

#include "rng.h"
#include "units.h"

template<typename U>
units::quantity<double, U> rng_float( units::quantity<double, U> lo,
                                      units::quantity<double, U> hi )
{
    return { rng_float( lo.value(), hi.value() ), U{} };
}

inline units::angle random_direction()
{
    return rng_float( 0_pi_radians, 2_pi_radians );
}

#endif // CATA_SRC_RNG_WITH_UNITS_H
