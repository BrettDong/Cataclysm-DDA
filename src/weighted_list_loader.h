#pragma once
#ifndef CATA_SRC_WEIGHTED_LIST_LOADER_H
#define CATA_SRC_WEIGHTED_LIST_LOADER_H

#include "json.h"
#include "weighted_list.h"

template<typename W, typename T>
void load_weighted_list( const JsonValue &jsv, weighted_list<W, T> &list, W default_weight )
{
    for( const JsonValue entry : jsv.get_array() ) {
        if( entry.test_array() ) {
            std::pair<T, W> p;
            entry.read( p, true );
            list.add( p.first, p.second );
        } else {
            T val;
            entry.read( val );
            list.add( val, default_weight );
        }
    }
}

#endif // CATA_SRC_WEIGHTED_LIST_LOADER_H
