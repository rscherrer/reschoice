// This script contains all the functions of the utl namespace

#include "utilities.hpp"

// Square function
double utl::sqr(const double &number) {

    return number * number;

}

// Numerical precision level
double utl::precis() {

    return 0.0000001; // 1e-6

}