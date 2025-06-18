#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test functionalities of the Individual class.

#include "../src/individual.hpp"
#include <boost/test/unit_test.hpp>

// Test that an individual is initialized properly
BOOST_AUTO_TEST_CASE(individualInitialization) {

    // Create individual
    Individual ind(1.0, 0.0);

    // Check attributes
    BOOST_CHECK_EQUAL(ind.getX(), 1.0);
    BOOST_CHECK(!ind.getHabitat());
    BOOST_CHECK(!ind.getEcotype());
    
}

// Test baseline choice probability
BOOST_AUTO_TEST_CASE(baselineProbability) {

    // Fifty fifty when resource abundance has no weight 
    BOOST_CHECK_EQUAL(ind::probbase(1.0, 1.1, 0.0), 0.5);

    // Relative resource proportion when full weight
    BOOST_CHECK_EQUAL(ind::probbase(1.0, 1.1, 1.0), 1.0 / 1.1);

    // In between 
    BOOST_CHECK_EQUAL(ind::probbase(1.0, 1.0, 0.5), 0.75);

}

// Test probability of choosing the best resource
BOOST_AUTO_TEST_CASE(probBestResource) {

    // Equal to the baseline probability in the absence of choice
    BOOST_CHECK_EQUAL(ind::probbest(0.8, 0.0), 0.8);

    // Equal to one with full choice
    BOOST_CHECK_EQUAL(ind::probbest(0.8, 1.0), 1.0);

    // In between
    BOOST_CHECK_EQUAL(ind::probbest(0.8, 0.5), 0.9);

}

// Test when choice is optimal
BOOST_AUTO_TEST_CASE(optimalChoice) {

    Individual ind(1.0, 0.0);
    ind.makeChoice(0.0, 1.0, 0.0, 1.0, 1.0, 1.0);
    BOOST_CHECK(ind.getChoice());

}

// Even when baseline choice depends on resource abundance
BOOST_AUTO_TEST_CASE(optimalChoiceWithResourceAbundance) {

    // Create a situation where one resource is almost absent but the
    // other is actually more advantageous.

    Individual ind(1.0, 0.0);
    ind.makeChoice(0.0, 1.0, 1.0, 1.0, 1.0, 0.1);
    BOOST_CHECK(ind.getChoice());

}

// Test that resource abundance weight directs random choice towards the most abundant resource
BOOST_AUTO_TEST_CASE(noChoiceButResourceAbundance) {

    // Situation where one resource is absent, there is no optimal choice and 
    // choice depends on resource abundance.

    Individual ind(1.0, 0.0);
    ind.makeChoice(1.0, 0.0, 1.0, 0.0, 0.0, 1.0);
    BOOST_CHECK(ind.getChoice());

}

// Test mutation
BOOST_AUTO_TEST_CASE(mutation) {

    Individual ind(0.0, 0.0);
    ind.mutate(0.01, 1.0);
    BOOST_CHECK_EQUAL(ind.getX(), 0.01);
    ind.mutate(-0.02, 1.0);
    BOOST_CHECK_EQUAL(ind.getX(), -0.01);

}

// Test dispersal
BOOST_AUTO_TEST_CASE(dispersal) {

    Individual ind(1.0, 0.0);
    BOOST_CHECK(!ind.getHabitat());
    ind.disperse();
    BOOST_CHECK(ind.getHabitat());

}

// Test rank assignment
BOOST_AUTO_TEST_CASE(rankAssignment) {

    Individual ind(1.0, 0.0);
    BOOST_CHECK(!ind.getRank());
    ind.setRank(30u);
    BOOST_CHECK_EQUAL(ind.getRank(), 30u);

}
