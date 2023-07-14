#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// On QtCreator:
// #define BOOST_TEST_DYN_LINK

#include "../src/simulation.hpp"
#include <boost/test/unit_test.hpp>

// Test that the simulation runs
BOOST_AUTO_TEST_CASE(useCase) {

    // Check that the main simulation function works
    BOOST_CHECK_EQUAL(simulate({"program_name"}), 0);

}

// Test that an individual is initialized properly
BOOST_AUTO_TEST_CASE(individualInitialization) {

    Individual ind(1.0);
    BOOST_CHECK(ind.isAlive());

}

// Test that an individual is dead after we kill it
BOOST_AUTO_TEST_CASE(individualIsDeadAfterBeingKilled) {

    Individual ind(1.0);
    ind.kill();
    BOOST_CHECK(!ind.isAlive());

}

// Test when choice is optimal
BOOST_AUTO_TEST_CASE(optimalChoice) {

    Individual ind(1.0);
    ind.setChoice(true, 1.0);
    BOOST_CHECK(ind.getChoice());

}

// Test mutation
BOOST_AUTO_TEST_CASE(mutation) {

    Individual ind(1.0);
    ind.mutate(0.01);
    BOOST_CHECK_EQUAL(ind.getX(), 0.01);
    ind.mutate(-0.02);
    BOOST_CHECK_EQUAL(ind.getX(), -0.01);

}