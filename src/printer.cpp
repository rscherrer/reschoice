// This script contains function definitions of the Printer class

#include "printer.hpp"

// Function to return a vector of default file names
std::vector<std::string> defaultFileNames() {

    return {

        "time",                                     // 0    time steps
        "resourceCensus",                           // 1    number of individuals feeding on each resource at each feeding round in each habitat
        "resourceMeanTraitValue",                   // 2    mean trait value of individuals feeding on each resource at each feeding round in each habitat
        "individualExpectedFitnessDifference",      // 3    expected fitness difference between the resources for each individual at each feeding round
        "individualChoice",                         // 4    resource choice made by each individual at each feeding round
        "individualRealizedFitness",                // 5    realized fitness gain of each individual after each feeding round
		"individualRank",                           // 6    position of each individual in the feeding queue at each feeding round
        "individualHabitat",                        // 7    habitat of each individual
        "individualTraitValue",                     // 8    trait value of each individual
        "individualTotalFitness",                   // 9    total fitness of each individual after all feeding rounds
        "individualEcotype",                        // 10   ecotype of each individual (relative to population average)
		"habitatCensus",                            // 11   number of individuals in each habitat
        "habitatMeanTraitValue",                    // 12   mean trait value in each habitat
		"ecologicalIsolation",                      // 13   ecological isolation statistic
        "spatialIsolation"                          // 14   spatial isolation statistic

    };
}

// Constructor
Printer::Printer(size_t wts) :
    fnames(defaultFileNames()),
    streams(std::vector<std::ofstream>(fnames.size()))
{

    // For each output file stream...
    for (size_t i = 0u; i < streams.size(); ++i) {

        assert(!streams[i].is_open());

        // If the current stream must be open (binary version of wts ends with a one)
        if (wts % 2u) {

            // Add extension to file name
            const std::string fname = fnames[i] + ".dat";
            
            // Open the corresponding file
            streams[i].open(fname.c_str(), std::ios::binary);

            // Check that the file was open properly
            if (!streams[i].is_open()) {

                std::string msg = "Unable to open output file " + fname;
                throw std::runtime_error(msg);

            }
        }

        // Bitshift the integer that represents what to save
        wts = wts >> 1u;

    }
}

// Function to close all the file streams
void Printer::close() {

    for (size_t f = 0u; f < streams.size(); ++f)
        streams[f].close();

}