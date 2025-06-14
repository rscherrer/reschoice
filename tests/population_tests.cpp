#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// These are tests that have to do with the Population class.

#include "testutils.hpp"
#include "../src/population.hpp"
#include <boost/test/unit_test.hpp>

// Test that a population initializes properly
BOOST_AUTO_TEST_CASE(populationInitializesProperly) {

    // Parameters
    Parameters pars;

    // Create a population
    Population pop(pars);

    // Check that individuals have been created
    BOOST_CHECK_EQUAL(pop.size(), pars.popsize);

    // Prepare to sum
    size_t sum = 0u;

    // Sum all demes
    for (size_t i = 0u; i < pop.size(); ++i)
        sum += pop.getHabitat(i);

    // Check that all individuals are in the first habitat
    BOOST_CHECK_EQUAL(sum, 0u);

}

// Test screen output
BOOST_AUTO_TEST_CASE(populationScreenOutput) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.popsize = 10u;
    pars.verbose = true;

    // Population
    Population pop(pars);

    // Printer
    Printer print({"foo", "bar"});

    // Capture output
    const std::string output = tst::captureOutput([&] { pop.cycle(print); });

    // Find relevant bits in output
    BOOST_CHECK(output.find("t = 0") != std::string::npos);

}

// Moving on works
BOOST_AUTO_TEST_CASE(populationMovesOn) {

    // Parameters
    Parameters pars;

    // Population
    Population pop(pars);

    // Check time is zero
    BOOST_CHECK_EQUAL(pop.getTime(), 0u);

    // Increment time
    pop.moveon();

    // Check time has changed
    BOOST_CHECK_EQUAL(pop.getTime(), 1u);

}

// Test that integration with printer works
BOOST_AUTO_TEST_CASE(populationCanPrint) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.popsize = 3u;
    pars.xstart = 0.5;
    pars.tsave = 1u;

    // Population
    Population pop(pars);

    // Printer
    Printer print({"time", "ecologicalIsolation", "habitatCensus", "individualTraitValue"});

    // Open the printer
    print.open();

    // Population cycle
    pop.cycle(print);

    // Close the printer
    print.close();

    // Read values back in
    std::vector<double> time = tst::read("time.dat");
    std::vector<double> ecologicalIsolation = tst::read("ecologicalIsolation.dat");
    std::vector<double> habitatCensus = tst::read("habitatCensus.dat");
    std::vector<double> individualTraitValue = tst::read("individualTraitValue.dat");

    // Check sizes
    BOOST_CHECK_EQUAL(time.size(), 1u);
    BOOST_CHECK_EQUAL(ecologicalIsolation.size(), 1u);
    BOOST_CHECK_EQUAL(habitatCensus.size(), 2u);
    BOOST_CHECK_EQUAL(individualTraitValue.size(), 3u);

    // Check time values
    BOOST_CHECK_EQUAL(time[0u], 0.0);

    // All individuals should be in first patch
    BOOST_CHECK_EQUAL(habitatCensus[0u], 3.0);
    BOOST_CHECK_EQUAL(habitatCensus[1u], 0.0);

    // Trait values should be the same
    BOOST_CHECK_EQUAL(individualTraitValue[0u], 0.5);
    BOOST_CHECK_EQUAL(individualTraitValue[1u], 0.5);
    BOOST_CHECK_EQUAL(individualTraitValue[2u], 0.5);

    // Remove files
    std::remove("time.dat");
    std::remove("ecologicalIsolation.dat");
    std::remove("habitatCensus.dat");
    std::remove("individualTraitValue.dat");

}

// Test resource discovery
BOOST_AUTO_TEST_CASE(resourceDiscovery) {

    // No resource discovered if the curve is flat (delta is zero)
    BOOST_CHECK_EQUAL(pop::discover(1.0, 0.0, 100.0), 0.0);

    // No resource discovered if no feeding happening
    BOOST_CHECK_EQUAL(pop::discover(1.0, 1.0, 0.0), 0.0);

    // Indistinguishable from saturated when the slope is very high
    BOOST_CHECK_EQUAL(pop::discover(1.0, 1000000.0, 100.0), 1.0);

}

// Test fitness
BOOST_AUTO_TEST_CASE(fitnessCalculation) {

    // No resource, no fitness
    BOOST_CHECK_EQUAL(pop::fitness(0.0, 1.0, 1.0), 0.0);

    // One gets all
    BOOST_CHECK_EQUAL(pop::fitness(1.0, 1.0, 1.0), 1.0);

    // One gets one tenth of the resource
    BOOST_CHECK_EQUAL(pop::fitness(1.0, 1.0, 10.0), 0.1);

    // One gets nothing if infinitely worse than everyone else
    BOOST_CHECK_EQUAL(pop::fitness(1.0, 0.0, 10.0), 0.0);

    // No feeders, no fitness
    BOOST_CHECK_EQUAL(pop::fitness(1.0, 0.0, 0.0, 0u), 0.0);

    // One gets all if no consumption but no-one else
    BOOST_CHECK_EQUAL(pop::fitness(1.0, 0.0, 0.0, 1u), 1.0);

    // Equal share if everybody has zero consumption
    BOOST_CHECK_EQUAL(pop::fitness(1.0, 0.0, 0.0, 10u), 0.1);

}

// Ecological isolation is correctly calculated
BOOST_AUTO_TEST_CASE(ecologicalIsolation) {

    BOOST_CHECK_EQUAL(stat::ei({{2u, 2u}, {0u, 0u}}, {{-2.0, 2.0}, {0.0, 0.0}}, {{2.0, 2.0}, {0.0, 0.0}}), 1.0);
    BOOST_CHECK_EQUAL(stat::ei({{2u, 0u}, {0u, 0u}}, {{-2.0, 0.0}, {0.0, 0.0}}, {{2.0, 0.0}, {0.0, 0.0}}), 0.0);

}

// Spatial isolation is correctly calculated
BOOST_AUTO_TEST_CASE(spatialIsolation) {

    BOOST_CHECK_EQUAL(stat::si({{1u, 0u}, {0u, 1u}}), 1.0);
    BOOST_CHECK_EQUAL(stat::si({{1u, 1u}, {1u, 1u}}), 0.0);
    BOOST_CHECK_EQUAL(stat::si({{1u, 1u}, {0u, 0u}}), 0.0);

}

// Population cycle with dispersal
BOOST_AUTO_TEST_CASE(populationCycleWithDispersal) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.dispersal = 1.0;
    pars.popsize = 2u;
 
     // Create a population
    Population pop(pars);

    // Printer
    Printer print({"foo", "bar"});

    // Check
    assert(pop.getHabitat(0u) == 0u);
    assert(pop.getHabitat(1u) == 0u);

    // Cycle
    pop.cycle(print);

    // Check that dispersal has occurred
    BOOST_CHECK_EQUAL(pop.getHabitat(0u), 1u);

}

// Population cycle with no dispersal
BOOST_AUTO_TEST_CASE(populationCycleWithNoDispersal) {

    // Parameters
    Parameters pars;

    // Tweak
    pars.dispersal = 0.0;
    pars.popsize = 2u;

    // Create a population
    Population pop(pars);

    // Printer
    Printer print({"foo", "bar"});

    // Check
    assert(pop.getHabitat(0u) == 0u);
    assert(pop.getHabitat(1u) == 0u);

    // Cycle
    pop.cycle(print);

    // Check that no dispersal has occurred
    BOOST_CHECK_EQUAL(pop.getHabitat(0u), 0u);

}

// Mutation occurs correctly (PROBABILISTIC)
BOOST_AUTO_TEST_CASE(populationCycleWithMutation) {
 
    // Parameters
    Parameters pars;

    // Tweak
    pars.mutrate = 1.0;
    pars.mutsdev = 0.1;
    pars.popsize = 10u;

    // Create a population
    Population pop(pars);

    // Printer
    Printer print({"foo", "bar"});

    // Cycle
    pop.cycle(print);

    // Check that the mean trait value has changed
    BOOST_CHECK(pop.getX(0u) != pars.xstart);

}