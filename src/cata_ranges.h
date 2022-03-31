#pragma once
#ifndef CATA_SRC_CATA_RANGES_H
#define CATA_SRC_CATA_RANGES_H

#include <utility>

#include <range/v3/view/any_view.hpp>

namespace cata
{
namespace ranges
{

template<typename T>
size_t size( ::ranges::any_view<T> &&view )
{
    size_t count = 0;
    for( auto it : view ) {
        (void)it;
        ++count;
    }
    return count;
}

template<typename T>
bool empty( ::ranges::any_view<T> &&view )
{
    return size( std::forward<::ranges::any_view<T>>( view ) ) == 0;
}

template<typename T>
T front( ::ranges::any_view<T> &&view )
{
    return *view.begin();
}

} // namespace ranges
} // namespace cata

#endif // CATA_SRC_CATA_VIEWS_H
