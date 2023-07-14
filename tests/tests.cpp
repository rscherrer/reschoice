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

// Test that it works when a parameter file is supplied
BOOST_AUTO_TEST_CASE(runWithParameterFile) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "popsize 10\n";
    file << "tradeoff 1\n";
    file << "beta 1\n";
    file << "delta 1\n";
    file << "res1 100\n";
    file << "res2 100\n";
    file << "nrounds 10\n";
    file << "mutrate 0.0001\n";
    file << "mutsdev 0.02\n";
    file << "tend 100\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 0);

}

// Test that it fails when too many arguments are provided
BOOST_AUTO_TEST_CASE(tooManyArgs) {

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameter.txt", "onetoomany.txt"}), 1);

}

// Test that it fails if the parameter file name is invalid
BOOST_AUTO_TEST_CASE(invalidFileName) {

    BOOST_CHECK_EQUAL(simulate({"program_name", "paraaameters.txt"}), 1);

}

// Should error when invalid parameter name
BOOST_AUTO_TEST_CASE(errorWhenInvalidParameterName) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "ahdggfhsgdhs 0\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Test that error when fixed population size is zero
BOOST_AUTO_TEST_CASE(errorWhenPopSizeIsZero) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "popsize 0\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Test that error when tradeoff is negative
BOOST_AUTO_TEST_CASE(errorWhenTradeOffIsNegative) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "tradeoff -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Test that error when choice accuracy is not between zero and one
BOOST_AUTO_TEST_CASE(errorWhenAccuracyIsNotBetweenZeroAndOne) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "beta -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Test that error when discovery rate is negative
BOOST_AUTO_TEST_CASE(errorWhenDiscoveryRateIsNegative) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "delta -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Test that error when resource amounts are negative
BOOST_AUTO_TEST_CASE(errorWhenResourceAmountsAreNegative) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "res1 -1\n";
    file << "res2 -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Test that error when zero feeding rounds
BOOST_AUTO_TEST_CASE(errorWhenZeroFeedingRounds) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "nrounds 0\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Test that error when mutation rate is not between zero and one
BOOST_AUTO_TEST_CASE(errorWhenMutationRateIsNotBetweenZeroAndOne) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "mutrate -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Test that error when mutational standard deviation is negative
BOOST_AUTO_TEST_CASE(errorWhenMutationalStandardDeviationIsNegative) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "mutsdev -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Test that error when simulation time is zero
BOOST_AUTO_TEST_CASE(errorWhenSimulationTimeIsZero) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "tend 0\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}


// Test that error when saving frequency is zero
BOOST_AUTO_TEST_CASE(errorWhenSavingFrequencyIsZero) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "tsave 0\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
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
    ind.mutate(0.01, 1.0);
    BOOST_CHECK_EQUAL(ind.getX(), 0.01);
    ind.mutate(-0.02, 1.0);
    BOOST_CHECK_EQUAL(ind.getX(), -0.01);

}