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

// Test that error when resource amount is negative
BOOST_AUTO_TEST_CASE(errorWhenResourceAmountIsNegative) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "resource -1\n";
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

// Test that error when dispersal rate is negative
BOOST_AUTO_TEST_CASE(errorWhenDispersalIsNegative) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "dispersal -1\n";
    file.close();

    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 1);
}

// Test that error when habitat symmetry is negative
BOOST_AUTO_TEST_CASE(errorWhenHabitatSymmetryIsNegative) {

    std::ofstream file;
    file.open("parameters.txt");
    file << "hsymmetry -1\n";
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

    Individual ind(1.0, 0.0);
    BOOST_CHECK(ind.isAlive());

}

// Test that an individual is dead after we kill it
BOOST_AUTO_TEST_CASE(individualIsDeadAfterBeingKilled) {

    Individual ind(1.0, 0.0);
    ind.kill();
    BOOST_CHECK(!ind.isAlive());

}

// Test when choice is optimal
BOOST_AUTO_TEST_CASE(optimalChoice) {

    Individual ind(1.0, 0.0);
    ind.makeChoice(0.0, 1.0, 1.0);
    BOOST_CHECK(ind.getChoice());

}

// Test birth
BOOST_AUTO_TEST_CASE(birth) {

    Individual ind(1.0, 0.0);
    ind.kill();
    BOOST_CHECK(!ind.isAlive());
    ind.isBorn();
    BOOST_CHECK(ind.isAlive());

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

// Test printer setup
BOOST_AUTO_TEST_CASE(printerSetUp) {

    // Only time.dat should be open
    Printer print(1u);
    BOOST_CHECK(print.streams[0u].is_open());
    BOOST_CHECK(!print.streams[1u].is_open());

}

// Closing of the streams works
BOOST_AUTO_TEST_CASE(streamClosure) {

    Printer print(1u);
    BOOST_CHECK(print.streams[0u].is_open());
    print.close();
    BOOST_CHECK(!print.streams[0u].is_open());

}

// Ecological isolation is correctly calculated
BOOST_AUTO_TEST_CASE(ecologicalIsolationIsCorrectlCalculated) {

    BOOST_CHECK_EQUAL(als::getEI({{2u, 2u}, {0u, 0u}}, {{-2.0, 2.0}, {0.0, 0.0}}, {{2.0, 2.0}, {0.0, 0.0}}), 1.0);
    BOOST_CHECK_EQUAL(als::getEI({{2u, 0u}, {0u, 0u}}, {{-2.0, 0.0}, {0.0, 0.0}}, {{2.0, 0.0}, {0.0, 0.0}}), 0.0);

}

// Spatial isolation is correctly calculated
BOOST_AUTO_TEST_CASE(spatialIsolationIsCorrectlyCalculated) {

    BOOST_CHECK_EQUAL(als::getSI({{1u, 0u}, {0u, 1u}}), 1.0);
    BOOST_CHECK_EQUAL(als::getSI({{1u, 1u}, {1u, 1u}}), 0.0);
    BOOST_CHECK_EQUAL(als::getSI({{1u, 1u}, {0u, 0u}}), 0.0);

}

// Test that parameters are saved properly
BOOST_AUTO_TEST_CASE(paramSavedProperly) {

    // Prepare a parameter file
    std::ofstream file;
    file.open("parameters.txt");
    file << "savepars 1\n";
    file.close();

    // First run where we save the parameters
    BOOST_CHECK_EQUAL(simulate({"program_name", "parameters.txt"}), 0);

    // Second run where we provide the saved parameters from the first one
    BOOST_CHECK_EQUAL(simulate({"program_name", "paramlog.txt"}), 0);

    // Check the seed was saved too
    std::ifstream infile;
    infile.open("paramlog.txt");
    std::string input;
    size_t seed = 0u;
    while (infile >> input) if (input == "seed") infile >> seed;
    infile.close();

    BOOST_CHECK(seed > 0u); // check that a random (nonzero) seed was saved

}

// Function to read in a binary output data file
template <typename T> std::vector<T> readBinary(const std::string &filename)
{
    // Open the input file
    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);

    // Prepare storage for values
    T x;
    std::vector<T> v;

    // If the file is open
    if (file.is_open()) {

        // Loop through the file until we reach the end of the file
        while(file) {

            // Read elements
            file.read((char *) &x, sizeof(T));

            // Exit if reaching the end of the file
            if (!file.gcount()) break;

            // Store elements
            v.push_back(x);

        }
    }

    // Close the file
    file.close();

    return v;

}

// Printer saves properly
BOOST_AUTO_TEST_CASE(printerSavesProperly) {

    Printer print(1u);
    print.save(123u, 0u);
    print.close();
    std::vector<double_t> timepoints = readBinary<double>("time.dat");
    BOOST_CHECK_EQUAL(timepoints.size(), 1u);
    BOOST_CHECK_EQUAL(timepoints[0u], 123.0);

}

// Test that output data are correctly written
BOOST_AUTO_TEST_CASE(outputDataAreCorrectlyWritten) {

    // Set up parameters with a known frequency of recording the data
    std::ofstream file;
    file.open("parameters.txt");
    file << "tend 10\n";
    file << "tsave 1\n";
    file.close();

    // Run a simulation
    simulate({"program_name", "parameters.txt"});

    // Read back one saved output data file
    std::vector<size_t> timepoints = readBinary<size_t>("time.dat");

    // Check the right number of entries have been saved
    BOOST_CHECK_EQUAL(timepoints.size(), 11u);

    // Now change the number of entries to save
    file.open("parameters.txt");
    file << "tend 10\n";
    file << "tsave 5\n";
    file.close();

    // Re-simulate
    simulate({"program_name", "parameters.txt"});

    // Read the new data back
    std::vector<size_t> newtimepoints = readBinary<size_t>("time.dat");

    // Check the new data does not have the same number of entries
    BOOST_CHECK(newtimepoints.size() < timepoints.size());
    BOOST_CHECK_EQUAL(newtimepoints.size(), 3u);

}

// Test that providing a file with what to save works
BOOST_AUTO_TEST_CASE(whatToSaveWorks) {

    // Create parameters
    std::ofstream file;
    file.open("parameters.txt");
    file << "tend 10\n";
    file << "tsave 2\n";
    file << "whattosave 24577\n"; // 110000000000001 should only save time, EI and SI
    file.close();

    // Simulate
    simulate({"program_name", "parameters.txt"});

    // Read back
    std::vector<size_t> timepoints = readBinary<size_t>("time.dat");
    std::vector<double> EI = readBinary<double>("ecologicalIsolation.dat");
    std::vector<double> SI = readBinary<double>("spatialIsolation.dat");

    // Check
    BOOST_CHECK_EQUAL(timepoints.size(), 6u);
    BOOST_CHECK_EQUAL(EI.size(), 6u);
    BOOST_CHECK_EQUAL(SI.size(), 6u);

}
