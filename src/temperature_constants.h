#pragma once
#ifndef CATA_SRC_TEMPERATURE_CONSTANTS_H
#define CATA_SRC_TEMPERATURE_CONSTANTS_H

#include "units.h"

// Temperature constants.
namespace temperatures
{
// temperature at which something starts is considered HOT.
constexpr units::temperature hot = units::from_fahrenheit( 100 ); // ~ 38 Celsius

// the "normal" temperature midpoint between cold and hot.
constexpr units::temperature normal = units::from_fahrenheit( 70 ); // ~ 21 Celsius

// Temperature inside an active fridge in Fahrenheit.
constexpr units::temperature fridge = units::from_fahrenheit( 37 ); // ~ 2.7 Celsius

// Temperature at which things are considered "cold".
constexpr units::temperature cold = units::from_fahrenheit( 40 ); // ~4.4 C

// Temperature inside an active freezer in Fahrenheit.
constexpr units::temperature freezer = units::from_celsius( -5 ); // -5 Celsius

// Temperature in which water freezes.
constexpr units::temperature freezing = units::from_celsius( 0 ); // 0 Celsius

// Temperature in which water boils.
constexpr units::temperature boiling = units::from_celsius( 100 ); // 100 Celsius
} // namespace temperatures

/**
 * Average annual temperature in F used for climate, weather and temperature calculation.
 * Average New England temperature = 43F/6C rounded to int.
*/
constexpr units::temperature AVERAGE_ANNUAL_TEMPERATURE = units::from_fahrenheit( 43 );

/**
 * Base starting spring temperature in F used for climate, weather and temperature calculation.
 * New England base spring temperature = 65F/18C rounded to int.
*/
constexpr units::temperature SPRING_TEMPERATURE = units::from_fahrenheit( 65 );

#endif // CATA_SRC_TEMPERATURE_CONSTANTS_H
