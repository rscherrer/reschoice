#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// On QtCreator:
// #define BOOST_TEST_DYN_LINK

#include "../src/hello.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testSayHello) {

    BOOST_CHECK_EQUAL(sayHello(), 0);

}
