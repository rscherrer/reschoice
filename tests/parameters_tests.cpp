#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the proper behavior of the Parameters class.

#include "testutils.hpp"
#include "../src/parameters.hpp"
#include <boost/test/unit_test.hpp>

// Test that parameters can be created
BOOST_AUTO_TEST_CASE(parametersCreated) {

    // Without input file should work fine
    Parameters pars;

    // Check a few default values
    BOOST_CHECK_EQUAL(pars.popsize, 10u);
    BOOST_CHECK_EQUAL(pars.xstart, 0.0);
    BOOST_CHECK_EQUAL(pars.tradeoff, 1.0);

}

// Parameters can update by reading from a file
BOOST_AUTO_TEST_CASE(readParameters)
{

    // Prepare content of parameter file
    std::ostringstream content;

    // Add lines
    content << "popsize 10\n";
    content << "xstart 1.0\n";
    content << "tradeoff 0.5\n";
    content << "alpha 0.3\n";
    content << "beta 0.7\n";
    content << "delta 0.2\n";
    content << "hsymmetry 0.8\n";
    content << "nrounds 5\n";
    content << "mutrate 0.01\n";
    content << "mutsdev 0.02\n";
    content << "dispersal 0.1\n";
    content << "tend 100\n";
    content << "tsave 20\n";
    content << "seed 12345\n";
    content << "savepars 1\n";
    content << "savedat 1\n";
    content << "verbose 1\n";
    content << "choose 0\n";
    content << "memsave 1.0\n";

    // Write the content to a file
    tst::write("parameters.txt", content.str());

    // Read the parameter file
    Parameters pars("parameters.txt");

    // Check that the parameters have been updated
    BOOST_CHECK_EQUAL(pars.popsize, 10u);
    BOOST_CHECK_EQUAL(pars.xstart, 1.0);
    BOOST_CHECK_EQUAL(pars.tradeoff, 0.5);
    BOOST_CHECK_EQUAL(pars.alpha, 0.3);
    BOOST_CHECK_EQUAL(pars.beta, 0.7);
    BOOST_CHECK_EQUAL(pars.delta, 0.2);
    BOOST_CHECK_EQUAL(pars.hsymmetry, 0.8);
    BOOST_CHECK_EQUAL(pars.nrounds, 5u);
    BOOST_CHECK_EQUAL(pars.mutrate, 0.01);
    BOOST_CHECK_EQUAL(pars.mutsdev, 0.02);
    BOOST_CHECK_EQUAL(pars.dispersal, 0.1);
    BOOST_CHECK_EQUAL(pars.tend, 100u);
    BOOST_CHECK_EQUAL(pars.tsave, 20u);
    BOOST_CHECK_EQUAL(pars.seed, 12345u);
    BOOST_CHECK(pars.savepars);
    BOOST_CHECK(pars.savedat);
    BOOST_CHECK(pars.verbose);
    BOOST_CHECK(!pars.choose);
    BOOST_CHECK_EQUAL(pars.memsave, 1.0);

    // Remove files
    std::remove("parameters.txt");

}

// Test that parameter reading fails when invalid parameters are provided
BOOST_AUTO_TEST_CASE(readInvalidParameters)
{

    // Write a file with invalid parameters
    tst::write("parameters.txt", "popsize 10\ninvalid 42\n");

    // Try to...
    tst::checkError([&]() {

        // Read the file
        Parameters pars("parameters.txt");

    }, "Invalid parameter: invalid in line 2 of file parameters.txt");

    // Remove files
    std::remove("parameters.txt");

}

// Test error upon invalid starting population size
BOOST_AUTO_TEST_CASE(readInvalidPopSize)
{

    // Write a file with invalid population size
    tst::write("p1.txt", "popsize 0\n");
    tst::write("p2.txt", "popsize 10 10\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Parameter popsize must be strictly positive in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too many values for parameter popsize in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid starting trait value
BOOST_AUTO_TEST_CASE(readInvalidXStart)
{

    // Write a file with invalid starting trait value
    tst::write("p1.txt", "xstart 10 10\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter xstart in line 1 of file p1.txt");

    // Remove files
    std::remove("p1.txt");

}

// Test error upon invalid tradeoff value
BOOST_AUTO_TEST_CASE(readInvalidTradeoff)
{

    // Write a file with invalid tradeoff value
    tst::write("p1.txt", "tradeoff -1\n");
    tst::write("p2.txt", "tradeoff 10 10\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Parameter tradeoff must be positive in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too many values for parameter tradeoff in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid resource abundance weight
BOOST_AUTO_TEST_CASE(readInvalidAlpha)
{

    // Write a file with invalid resource abundance weight
    tst::write("p1.txt", "alpha -0.5\n");
    tst::write("p2.txt", "alpha 0.5 0.5\n");
    tst::write("p3.txt", "alpha 1.5\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Parameter alpha must be between 0 and 1 in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too many values for parameter alpha in line 1 of file p2.txt");
    tst::checkError([&]() { Parameters pars("p3.txt"); }, "Parameter alpha must be between 0 and 1 in line 1 of file p3.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");
    std::remove("p3.txt");

}

// Test error upon invalid resource choice accuracy
BOOST_AUTO_TEST_CASE(readInvalidBeta)
{

    // Write a file with invalid resource choice accuracy
    tst::write("p1.txt", "beta -0.5\n");
    tst::write("p2.txt", "beta 0.5 0.5\n");
    tst::write("p3.txt", "beta 1.5\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Parameter beta must be between 0 and 1 in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too many values for parameter beta in line 1 of file p2.txt");
    tst::checkError([&]() { Parameters pars("p3.txt"); }, "Parameter beta must be between 0 and 1 in line 1 of file p3.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");
    std::remove("p3.txt");

}

// Test error upon invalid resource discovery rate
BOOST_AUTO_TEST_CASE(readInvalidDelta)
{

    // Write a file with invalid resource discovery rate
    tst::write("p1.txt", "delta -0.5\n");
    tst::write("p2.txt", "delta 10 10\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Parameter delta must be positive in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too many values for parameter delta in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid habitat symmetry parameter
BOOST_AUTO_TEST_CASE(readInvalidHsymmetry)
{

    // Write a file with invalid habitat symmetry parameter
    tst::write("p1.txt", "hsymmetry -0.5\n");
    tst::write("p2.txt", "hsymmetry 0.5 0.5\n");
    tst::write("p3.txt", "hsymmetry 1.5\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Parameter hsymmetry must be between 0 and 1 in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too many values for parameter hsymmetry in line 1 of file p2.txt");
    tst::checkError([&]() { Parameters pars("p3.txt"); }, "Parameter hsymmetry must be between 0 and 1 in line 1 of file p3.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");
    std::remove("p3.txt");

}

// Test error upon invalid number of rounds
BOOST_AUTO_TEST_CASE(readInvalidNRounds)
{

    // Write a file with invalid number of rounds
    tst::write("p1.txt", "nrounds 0\n");
    tst::write("p2.txt", "nrounds 10 10\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Parameter nrounds must be strictly positive in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too many values for parameter nrounds in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid mutation rate
BOOST_AUTO_TEST_CASE(readInvalidMutRate)
{

    // Write a file with invalid mutation rate
    tst::write("p1.txt", "mutrate -0.5\n");
    tst::write("p2.txt", "mutrate 0.5 0.5\n");
    tst::write("p3.txt", "mutrate 1.5\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Parameter mutrate must be between 0 and 1 in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too many values for parameter mutrate in line 1 of file p2.txt");
    tst::checkError([&]() { Parameters pars("p3.txt"); }, "Parameter mutrate must be between 0 and 1 in line 1 of file p3.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");
    std::remove("p3.txt");

}

// Test error upon invalid mutational standard deviation
BOOST_AUTO_TEST_CASE(readInvalidMutSdev)
{

    // Write a file with invalid mutational standard deviation
    tst::write("p1.txt", "mutsdev -0.5\n");
    tst::write("p2.txt", "mutsdev 10 10\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Parameter mutsdev must be positive in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too many values for parameter mutsdev in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid dispersal rate
BOOST_AUTO_TEST_CASE(readInvalidDispersalRate)
{

    // Write a file with invalid dispersal rate
    tst::write("p1.txt", "dispersal -0.5\n");
    tst::write("p2.txt", "dispersal 0.5 0.5\n");
    tst::write("p3.txt", "dispersal 1.5\n");

    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Parameter dispersal must be between 0 and 1 in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Too many values for parameter dispersal in line 1 of file p2.txt");
    tst::checkError([&]() { Parameters pars("p3.txt"); }, "Parameter dispersal must be between 0 and 1 in line 1 of file p3.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");
    std::remove("p3.txt");

}

// Test error upon invalid end time
BOOST_AUTO_TEST_CASE(readInvalidTEnd)
{

    // Write a file with invalid end time
    tst::write("p1.txt", "tend 10 10");
    tst::write("p2.txt", "tend 0");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter tend in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter tend must be strictly positive in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid save time
BOOST_AUTO_TEST_CASE(readInvalidTSave)
{

    // Write a file with invalid save time
    tst::write("p1.txt", "tsave 10 10");
    tst::write("p2.txt", "tsave 0");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter tsave in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter tsave must be strictly positive in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid saving memory
BOOST_AUTO_TEST_CASE(readInvalidMemSave)
{

    // Write a file with invalid saving memory
    tst::write("p1.txt", "memsave 0.5 0.5");
    tst::write("p2.txt", "memsave 0.00000001");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter memsave in line 1 of file p1.txt");
    tst::checkError([&]() { Parameters pars("p2.txt"); }, "Parameter memsave must be enough MB to store a double in line 1 of file p2.txt");

    // Remove files
    std::remove("p1.txt");
    std::remove("p2.txt");

}

// Test error upon invalid seed
BOOST_AUTO_TEST_CASE(readInvalidSeed)
{

    // Write a file with invalid seed
    tst::write("p1.txt", "seed 10 10");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter seed in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
}

// Test that error upon invalid parameter saving flag
BOOST_AUTO_TEST_CASE(readInvalidSavePars)
{

    // Write a file with invalid parameter saving flag
    tst::write("p1.txt", "savepars 1 1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter savepars in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
}

// Test that error upon invalid data saving flag
BOOST_AUTO_TEST_CASE(readInvalidSaveDat)
{

    // Write a file with invalid data saving flag
    tst::write("p1.txt", "savedat 1 1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter savedat in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
}

// Test that error upon invalid choice flag
BOOST_AUTO_TEST_CASE(readInvalidChoose)
{

    // Write a file with invalid choice flag
    tst::write("p1.txt", "choose 1 1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter choose in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
}

// Test that error upon invalid verbosity flag
BOOST_AUTO_TEST_CASE(readInvalidVerbose)
{

    // Write a file with invalid verbosity flag
    tst::write("p1.txt", "verbose 1 1");
    
    // Check
    tst::checkError([&]() { Parameters pars("p1.txt"); }, "Too many values for parameter verbose in line 1 of file p1.txt");
    
    // Remove files
    std::remove("p1.txt");
    
}

// Test that the parameter saving function works
BOOST_AUTO_TEST_CASE(parameterSavingWorks) {

    // Create parameters
    Parameters pars1;

    // Modify
    pars1.popsize = 42u;

    // Save
    pars1.save("parameters.txt");

    // Overwrite the other
    Parameters pars2("parameters.txt");

    // Make sure the values have been properly replaced
    BOOST_CHECK_EQUAL(pars2.popsize, 42u);

    // Remove files
    std::remove("parameters.txt");

}

// Test when parameter saving does not work
BOOST_AUTO_TEST_CASE(errorWhenSavingParameters) {

    // Check
    tst::checkError([&] {

        // Create parameters
        Parameters pars;

        // Save
        pars.save("");

    }, "Unable to open file ");

}