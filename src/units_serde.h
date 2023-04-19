#pragma once
#ifndef CATA_SRC_UNITS_SERDE_H
#define CATA_SRC_UNITS_SERDE_H

#include <string>
#include <string_view>
#include <vector>

#include "json.h"
#include "units.h"

namespace detail
{
template<typename T, typename Error>
T read_from_json_string_common( const std::string_view s,
                                const std::vector<std::pair<std::string, T>> &units, Error &&error )
{
    size_t i = 0;
    // returns whether we are at the end of the string
    const auto skip_spaces = [&]() {
        while( i < s.size() && s[ i ] == ' ' ) {
            ++i;
        }
        return i >= s.size();
    };
    const auto get_unit = [&]() {
        if( skip_spaces() ) {
            error( "invalid quantity string: missing unit", i );
        }
        for( const std::pair<std::string, T> &pair : units ) {
            const std::string &unit = pair.first;
            if( s.size() >= unit.size() + i && s.compare( i, unit.size(), unit ) == 0 ) {
                i += unit.size();
                return pair.second;
            }
        }
        error( "invalid quantity string: unknown unit", i );
        // above always throws but lambdas cannot be marked [[noreturn]]
        throw std::string( "Exceptionally impossible" );
    };

    if( skip_spaces() ) {
        error( "invalid quantity string: empty string", i );
        // above always throws but lambdas cannot be marked [[noreturn]]
        throw std::string( "Exceptionally impossible" );
    }
    T result{};
    do {
        int sign_value = +1;
        if( s[ i ] == '-' ) {
            sign_value = -1;
            ++i;
        } else if( s[ i ] == '+' ) {
            ++i;
        }
        if( i >= s.size() || !isdigit( s[ i ] ) ) {
            error( "invalid quantity string: number expected", i );
            // above always throws but lambdas cannot be marked [[noreturn]]
            throw std::string( "Exceptionally impossible" );
        }
        int value = 0;
        for( ; i < s.size() && isdigit( s[ i ] ); ++i ) {
            value = value * 10 + ( s[ i ] - '0' );
        }
        result += sign_value * value * get_unit();
    } while( !skip_spaces() );
    return result;
}
} // namespace detail

template<typename T>
T read_from_json_string( const JsonValue &jv, const std::vector<std::pair<std::string, T>> &units )
{
    const auto error = [&]( const char *const msg, size_t offset ) {
        jv.throw_error( offset, msg );
    };

    const std::string s = jv;

    return detail::read_from_json_string_common<T>( s, units, error );
}

template<typename T>
void dump_to_json_string( T t, JsonOut &jsout,
                          const std::vector<std::pair<std::string, T>> &units )
{
    // deduplicate unit strings and choose the shortest representations
    std::map<T, std::string> sorted_units;
    for( const std::pair<std::string, T> &p : units ) {
        const auto it = sorted_units.find( p.second );
        if( it != sorted_units.end() ) {
            if( p.first.length() < it->second.length() ) {
                it->second = p.first;
            }
        } else {
            sorted_units.emplace( p.second, p.first );
        }
    }
    std::string str;
    bool written = false;
    for( auto it = sorted_units.rbegin(); it != sorted_units.rend(); ++it ) {
        const int val = static_cast<int>( t / it->first );
        if( val != 0 ) {
            if( written ) {
                str += ' ';
            }
            int tmp = val;
            if( tmp < 0 ) {
                str += '-';
                tmp = -tmp;
            }
            const size_t val_beg = str.size();
            while( tmp != 0 ) {
                str += static_cast<char>( '0' + tmp % 10 );
                tmp /= 10;
            }
            std::reverse( str.begin() + val_beg, str.end() );
            str += ' ';
            str += it->second;
            written = true;
            t -= it->first * val;
        }
    }
    if( str.empty() ) {
        str = "0 " + sorted_units.begin()->second;
    }
    jsout.write( str );
}

#endif // CATA_SRC_UNITS_SERDE_H
