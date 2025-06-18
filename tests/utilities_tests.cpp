#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the functions of namespace Utility.

#include "testutils.hpp"
#include "../src/utilities.hpp"
#include <boost/test/unit_test.hpp>

// Test that square function works correctly
BOOST_AUTO_TEST_CASE(squareWorks) {

    // Check some known values
    BOOST_CHECK_EQUAL(utl::sqr(2.0), 4.0);
    BOOST_CHECK_EQUAL(utl::sqr(-3.0), 9.0);
    BOOST_CHECK_EQUAL(utl::sqr(0.0), 0.0);
    BOOST_CHECK_EQUAL(utl::sqr(1.0), 1.0);
    
}

// Test that the correction function works correctly
BOOST_AUTO_TEST_CASE(correctWorks) {

    // Check some known values
    BOOST_CHECK_EQUAL(utl::correct(0.0), 0.0);
    BOOST_CHECK_EQUAL(utl::correct(-1e-07), 0.0);
    BOOST_CHECK_EQUAL(utl::correct(1e-07), 1e-07);
    BOOST_CHECK_EQUAL(utl::correct(-1e-06), -1e-06);
    BOOST_CHECK_EQUAL(utl::correct(1e-06), 1e-06);
    
}

// Test that pi function returns the correct value
BOOST_AUTO_TEST_CASE(piWorks) {

    // Check that the value of pi is correct
    BOOST_CHECK_CLOSE(utl::pi(), 3.14159265358979323846, 1e-10);
    
}