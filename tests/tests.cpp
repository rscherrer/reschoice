#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test all the uses and misuses of the program. These mostly have to
// with calling the program, passing arguments, reading from and writing to
// files, and error handling.

#include "testutils.hpp"
#include "../src/MAIN.hpp"
#include <boost/test/unit_test.hpp>

// Test that the simulation runs
BOOST_AUTO_TEST_CASE(useCase) {

    // Check that the program runs
    BOOST_CHECK_NO_THROW(doMain({"program"}));

}

// Test that it fails when too many arguments are provided
BOOST_AUTO_TEST_CASE(abuseTooManyArgs) {

    // Check error
    tst::checkError([&] { 
        doMain({"program", "parameter.txt", "onetoomany.txt"});
    }, "Too many arguments provided");

}

// Test that the simulation runs with a parameter file
BOOST_AUTO_TEST_CASE(useCaseWithParameterFile) {

    // Write a parameter file
    tst::write("parameters.txt", "popsize 9");

    // Check that the program runs
    BOOST_CHECK_NO_THROW(doMain({"program", "parameters.txt"}));

    // Cleanup
    std::remove("parameters.txt");

}

// Test that error when invalid parameter file
BOOST_AUTO_TEST_CASE(abuseInvalidParameterFile) {

    // Check error
    tst::checkError([&] {
        doMain({"program", "nonexistent.txt"});
    }, "Unable to open file nonexistent.txt");

}

// Test that it works when saving the parameters
BOOST_AUTO_TEST_CASE(useCaseWithParameterSaving) {

    // Write a parameter file specifying to save parameters
    tst::write("parameters.txt", "savepars 1");

    // Run a simulation
    doMain({"program", "parameters.txt"});

    // Read the saved parameters
    Parameters pars1("paramlog.txt");

    // Rerun the simulation with the saved parameters
    doMain({"program", "paramlog.txt"});

    // Read the overwritten parameters
    Parameters pars2("paramlog.txt");

    // Check that the (clock-generated) seed is the same
    BOOST_CHECK_EQUAL(pars1.seed, pars2.seed);

    // Cleanup
    std::remove("parameters.txt");
    std::remove("paramlog.txt");

}

// Test that it works when the user can choose which data to save
BOOST_AUTO_TEST_CASE(useCaseUserDefinedOutput) {

    // Write an output request file
    tst::write("whattosave.txt", "time");

    // Write a parameter file
    tst::write("parameters.txt", "savedat 1\nchoose 1\ntend 10\ntsave 1");

    // Run the simulation
    doMain({"program", "parameters.txt"});

    // Read the data if they exist
    const std::vector<double> values = tst::read("time.dat");

    // Check
    assert(!values.empty());

    // Check their values
    for (auto i : values)
        BOOST_CHECK_EQUAL(values[i], i);
    
    // Check that no other output file was read
    tst::checkError([&] { 
        tst::read("habitatCensus.dat"); 
    }, "Unable to open file habitatCensus.dat");
    
    // Cleanup
    std::remove("parameters.txt");
    std::remove("whattosave.txt");
    std::remove("time.dat");
    
}

// Test that error when wrong output request file
BOOST_AUTO_TEST_CASE(abuseWrongOutputRequestFile) {

    // Write a parameter file
    tst::write("parameters.txt", "savedat 1\nchoose 1");

    // Check error if output request file not present
    tst::checkError([&] {
        doMain({"program", "parameters.txt"});
    }, "Unable to open file whattosave.txt");

    // Cleanup
    std::remove("parameters.txt");

}

// Test that all outputs are saved if no choice is made
BOOST_AUTO_TEST_CASE(useCaseAllOutputsIfNoChoice) {

    // Write a parameter file with data saving but no choice
    tst::write("parameters.txt", "popsize 5\nnrounds 2\ntend 10\ntsave 1\nsavedat 1\nchoose 0");

    // Run the simulation
    doMain({"program", "parameters.txt"});

    // Check that all output files can be read
    BOOST_CHECK_NO_THROW(tst::read("time.dat"));
    BOOST_CHECK_NO_THROW(tst::read("resourceCensus.dat"));
    BOOST_CHECK_NO_THROW(tst::read("resourceMeanTraitValue.dat"));
    BOOST_CHECK_NO_THROW(tst::read("individualExpectedFitnessDifference.dat"));
    BOOST_CHECK_NO_THROW(tst::read("individualChoice.dat"));
    BOOST_CHECK_NO_THROW(tst::read("individualRealizedFitness.dat"));
    BOOST_CHECK_NO_THROW(tst::read("individualRank.dat"));
    BOOST_CHECK_NO_THROW(tst::read("individualHabitat.dat"));
    BOOST_CHECK_NO_THROW(tst::read("individualTraitValue.dat"));
    BOOST_CHECK_NO_THROW(tst::read("individualTotalFitness.dat"));
    BOOST_CHECK_NO_THROW(tst::read("individualEcotype.dat"));
    BOOST_CHECK_NO_THROW(tst::read("habitatCensus.dat"));
    BOOST_CHECK_NO_THROW(tst::read("habitatMeanTraitValue.dat"));
    BOOST_CHECK_NO_THROW(tst::read("ecologicalIsolation.dat"));
    BOOST_CHECK_NO_THROW(tst::read("spatialIsolation.dat"));

    // Check some values
    BOOST_CHECK_EQUAL(tst::read("time.dat").size(), 11u);
    BOOST_CHECK_EQUAL(tst::read("resourceCensus.dat").size(), 88u);
    BOOST_CHECK_EQUAL(tst::read("resourceMeanTraitValue.dat").size(), 88u);
    BOOST_CHECK_EQUAL(tst::read("individualExpectedFitnessDifference.dat").size(), 110u);
    BOOST_CHECK_EQUAL(tst::read("individualChoice.dat").size(), 110u);
    BOOST_CHECK_EQUAL(tst::read("individualRealizedFitness.dat").size(), 110u);
    BOOST_CHECK_EQUAL(tst::read("individualRank.dat").size(), 110u);
    BOOST_CHECK_EQUAL(tst::read("individualHabitat.dat").size(), 55u);
    BOOST_CHECK_EQUAL(tst::read("individualTraitValue.dat").size(), 55u);
    BOOST_CHECK_EQUAL(tst::read("individualTotalFitness.dat").size(), 55u);
    BOOST_CHECK_EQUAL(tst::read("individualEcotype.dat").size(), 55u);
    BOOST_CHECK_EQUAL(tst::read("habitatCensus.dat").size(), 22u);
    BOOST_CHECK_EQUAL(tst::read("habitatMeanTraitValue.dat").size(), 22u);
    BOOST_CHECK_EQUAL(tst::read("ecologicalIsolation.dat").size(), 11u);
    BOOST_CHECK_EQUAL(tst::read("spatialIsolation.dat").size(), 11u);

    // Cleanup
    std::remove("parameters.txt");
    std::remove("time.dat");
    std::remove("resourceCensus.dat");
    std::remove("resourceMeanTraitValue.dat");
    std::remove("individualExpectedFitnessDifference.dat");
    std::remove("individualChoice.dat");
    std::remove("individualRealizedFitness.dat");
    std::remove("individualRank.dat");
    std::remove("individualHabitat.dat");
    std::remove("individualTraitValue.dat");
    std::remove("individualTotalFitness.dat");
    std::remove("individualEcotype.dat");
    std::remove("habitatCensus.dat");
    std::remove("habitatMeanTraitValue.dat");
    std::remove("ecologicalIsolation.dat");
    std::remove("spatialIsolation.dat");

}

// Test that nothing is saved if no data saving
BOOST_AUTO_TEST_CASE(useCaseNothingIsSaved) {

    // Write a parameter file specifying no data saving
    tst::write("parameters.txt", "savedat 0");

    // Run the simulation
    doMain({"program", "parameters.txt"});

    // Check that none of the possible output files are present
    tst::checkError([&] {tst::read("time.dat");}, "Unable to open file time.dat");
    tst::checkError([&] {tst::read("resourceCensus.dat");}, "Unable to open file resourceCensus.dat");
    tst::checkError([&] {tst::read("individualExpectedFitnessDifference.dat");}, "Unable to open file individualExpectedFitnessDifference.dat");

    // Cleanup
    std::remove("parameters.txt");

}

// Test that it works when verbose is on
BOOST_AUTO_TEST_CASE(useCaseWithVerbose) {

    // Write a parameter file with verbose
    tst::write("parameters.txt", "verbose 1\ntend 1\npopsize 10");

    // Capture output
    const std::string output = tst::captureOutput([&] { doMain({"program", "parameters.txt"}); });

    // Find relevant bits in output
    BOOST_CHECK(output.find("Simulation started") != std::string::npos);
    BOOST_CHECK(output.find("t = 0") != std::string::npos);
    BOOST_CHECK(output.find("Simulation ended") != std::string::npos);

    // Cleanup
    std::remove("parameters.txt");

}