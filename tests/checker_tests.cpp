#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the checking functions.

#include "testutils.hpp"
#include "../src/checker.hpp"
#include <boost/test/unit_test.hpp>

// Test the proportion checking function
BOOST_AUTO_TEST_CASE(isProportion) {

    // Known values
    BOOST_CHECK_EQUAL(chk::proportion(0.0), "");
    BOOST_CHECK_EQUAL(chk::proportion(0.5), "");
    BOOST_CHECK_EQUAL(chk::proportion(1.0), "");
    BOOST_CHECK_EQUAL(chk::proportion(-0.5), "must be between 0 and 1");
    BOOST_CHECK_EQUAL(chk::proportion(1.5), "must be between 0 and 1");

}

// Test the one-to-thousand checking function
BOOST_AUTO_TEST_CASE(isOneToThousand) {

    // Known values
    BOOST_CHECK_EQUAL(chk::onetothousand(0u), "must be between 1 and 1000");
    BOOST_CHECK_EQUAL(chk::onetothousand(1u), "");
    BOOST_CHECK_EQUAL(chk::onetothousand(1000u), "");
    BOOST_CHECK_EQUAL(chk::onetothousand(1001u), "must be between 1 and 1000");

}

// Test the memory checking function
BOOST_AUTO_TEST_CASE(isEnoughMB) {

    // Known values
    BOOST_CHECK_EQUAL(chk::enoughmb(0.0), "must be enough MB to store a double");
    BOOST_CHECK_EQUAL(chk::enoughmb(0.1), "");
    BOOST_CHECK_EQUAL(chk::enoughmb(1.0), "");
    BOOST_CHECK_EQUAL(chk::enoughmb(0.000008), "");
    BOOST_CHECK_EQUAL(chk::enoughmb(0.000007), "must be enough MB to store a double");

}

// Test the positive checking function
BOOST_AUTO_TEST_CASE(isPositive) {

    // Known values
    BOOST_CHECK_EQUAL(chk::positive(0.0), "");
    BOOST_CHECK_EQUAL(chk::positive(1.0), "");
    BOOST_CHECK_EQUAL(chk::positive(-1.0), "must be positive");
    BOOST_CHECK_EQUAL(chk::positive(-0.5), "must be positive");

}

// Test the strict positive checking function
BOOST_AUTO_TEST_CASE(isStrictPositive) {

    // Known values
    BOOST_CHECK_EQUAL(chk::strictpos(0.0), "must be strictly positive");
    BOOST_CHECK_EQUAL(chk::strictpos(1.0), "");
    BOOST_CHECK_EQUAL(chk::strictpos(-1.0), "must be strictly positive");
    BOOST_CHECK_EQUAL(chk::strictpos(-0.5), "must be strictly positive");
    BOOST_CHECK_EQUAL(chk::strictpos(1u), "");
    BOOST_CHECK_EQUAL(chk::strictpos(0u), "must be strictly positive");

}

// Test the strict order checking function
BOOST_AUTO_TEST_CASE(isStrictOrder) {

    // Known values
    BOOST_CHECK_EQUAL(chk::strictorder(std::vector<double>{1.0, 2.0, 3.0}), "");
    BOOST_CHECK_EQUAL(chk::strictorder(std::vector<double>{1.0, 3.0, 2.0}), "must be in strictly increasing order");
    BOOST_CHECK_EQUAL(chk::strictorder(std::vector<double>{1.0, 2.0, 1.5}), "must be in strictly increasing order");
    BOOST_CHECK_EQUAL(chk::strictorder(std::vector<double>{1.0, 2.0, 2.0}), "must be in strictly increasing order");

}
