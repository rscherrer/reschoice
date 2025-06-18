#ifndef RESCHOICE_UTILITIES_HPP
#define RESCHOICE_UTILITIES_HPP

// This header contains a set of miscellaneous utility functions

namespace utl
{

    // Utility functions
    inline double sqr(const double &x) { return x * x; }
    inline double correct(const double &x) { return x < 0.0 && x > -1e-06 ? 0.0 : x; }
    inline double pi() { const double x = 3.14159265358979323846; return x; }
    
}

#endif

