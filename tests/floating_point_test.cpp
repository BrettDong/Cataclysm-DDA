#include "cata_catch.h"
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>

volatile double dA50 = 8.0;
volatile double dAto95 = 3.0;
volatile double dAlpha = 1.0;

static double calculate( int param )
{
    volatile double dRet = 0.0;
    volatile double dTemp = ( dA50 - param ) / dAto95;
    dRet = dAlpha / ( 1.0 + pow( 19.0, dTemp ) );
    return dRet;
}

static void check_float_point_across_threads()
{
    std::ostringstream oss;
    oss << std::setprecision( 20 );
    volatile int param = 1;
    oss << calculate( param );
    CHECK( oss.str() == "0.0010370292068795884059" );
    oss.str( "" );
    oss.clear();
    std::thread thread( [&oss, &param]() {
        oss << std::setprecision( 20 );
        oss << calculate( param );
    } );
    thread.join();
    CHECK( oss.str() == "0.0010370292068795884059" );
}

TEST_CASE( "floating_point_across_threads" )
{
    check_float_point_across_threads();
}