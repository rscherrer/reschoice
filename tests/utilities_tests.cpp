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
