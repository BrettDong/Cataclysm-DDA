#pragma once
#ifndef CATA_SRC_CATA_PREDICATES_H
#define CATA_SRC_CATA_PREDICATES_H

/** Used as a default filter in various functions */
template<typename T>
bool return_true( const T & )
{
    return true;
}

template<typename T>
bool return_false( const T & )
{
    return false;
}

#endif // CATA_SRC_CATA_PREDICATES_H
