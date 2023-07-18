// This script contains all the functions of the stf namespace

#include "printer.hpp"

// Function to add and open output file streams to a vector of streams
void stf::open(
    std::vector<std::shared_ptr<std::ofstream> > &outfiles,
    const std::vector<std::string> &filenames
) {

    // Reserve space for enough output files
    outfiles.reserve(filenames.size());

    // For each file...
    for (size_t f = 0u; f < filenames.size(); ++f) {

        // Add extension to the file name
        const std::string filename = filenames[f] + ".dat";

        // Create a new output stream
        std::shared_ptr<std::ofstream> out(new std::ofstream);

        // Open the output stream (data are written in binary format)
        out->open(filename.c_str(), std::ios::binary);

        // Check
        if (!out->is_open()) {
            std::string msg = "Unable to open output file " + filename;
            throw std::runtime_error(msg);
        }

        // Add the stream to the vector of streams
        outfiles.push_back(out);

    }
}

// Function to close all the file streams
void stf::close(std::vector<std::shared_ptr<std::ofstream> > &outfiles) {

    for (size_t f = 0u; f < outfiles.size(); ++f)
        outfiles[f]->close();

}

// Function to check that the provided file names are good
void stf::check(const std::vector<std::string> &v1, const std::vector<std::string> &v2) {

    // v1 is the vector of provided file names
    // v2 is the vector of default file names

    if (!v1.size()) return;

    bool found = false;

    // For each supplied file name...
    for (size_t i = 0u; i < v1.size(); ++i) {

        // Match it to each file name in the default list...
        for (size_t j = 0u; j < v2.size(); ++j) {

            // If there is one match, the provided name is valid
            if (v1[i] == v2[j]) {
                found = true;
                break;
            }
        }

        // Error if the file name is unknown
        if (!found) throw std::runtime_error("Invalid file name provided in whattosave");
    }
}

// Function to save time
void stf::saveTime(const size_t &t, std::shared_ptr<std::ofstream> &file) {

    const double t_ = static_cast<double>(t);
    file->write((char *) &t_, sizeof(double));

}

// Function to save the expected fitness difference of an individual during a feeding round
void stf::saveIndividualExpectedFitnessDifference(const double &diff, std::shared_ptr<std::ofstream> &file) {

    file->write((char *) &diff, sizeof(double));

}

// Function to save the choice made by an individual during a feeding round
void stf::saveIndividualChoice(const bool &choice, std::shared_ptr<std::ofstream> &file) {

    const double choice_ = static_cast<double>(choice);
	file->write((char *) &choice_, sizeof(double));

}

// Function to save the number of individuals feeding on each resource in one feeding round
void stf::saveResourceCensusFile(const size_t &n1, const size_t &n2, std::shared_ptr<std::ofstream> &file) {

    const double n1_ = static_cast<double>(n1);
	const double n2_ = static_cast<double>(n2);
	file->write((char *) &n1_, sizeof(double));
	file->write((char *) &n2_, sizeof(double));

}

// Function to save the mean trait value of individuals feeding on each resource during a feeding round
void stf::saveResourceMeanTraitValue(const double &mean1, const double &mean2, std::shared_ptr<std::ofstream> &file) {

    file->write((char *) &mean1, sizeof(double));
	file->write((char *) &mean2, sizeof(double));

}

// Function to save the realized fitness of an individual during a feeding round
void stf::saveIndividualRealizedFitness(const double &fit, std::shared_ptr<std::ofstream> &file) {

    file->write((char *) &fit, sizeof(double));

}

// Function to save the habitat of an individual
void stf::saveIndividualHabitat(const bool &hab, std::shared_ptr<std::ofstream> &file) {

    const double habitat_ = static_cast<double>(hab);
	file->write((char *) &habitat_, sizeof(double));

}

// Function to save the trait value of an individual
void stf::saveIndividualTraitValue(const double &x, std::shared_ptr<std::ofstream> &file) {

    file->write((char *) &x, sizeof(double));

}

// Function to save the total fitness of an individual
void stf::saveIndividualTotalFitness(const double &fit, std::shared_ptr<std::ofstream> &file) {

    file->write((char *) &fit, sizeof(double));

}

// Function to save a double
void stf::save(const double &x, std::shared_ptr<std::ofstream> &file) {
    
    file->write((char *) &x, sizeof(double));

}

// Function to save a boolean
void stf::save(const bool &x, std::shared_ptr<std::ofstream> &file) {

    const double x_ = static_cast<double>(x);
    file->write((char *) &x_, sizeof(double));

}

// Function to save a size-type
void stf::save(const size_t &x, std::shared_ptr<std::ofstream> &file) {

    const double x_ = static_cast<double>(x);
    file->write((char *) &x_, sizeof(double));

}

// Function to save a value to output file
template <typename T> void stf::save(const T &x, std::shared_ptr<std::ofstream> &file) {

    const double x_ = static_cast<double>(x);
    file->write((char *) &x_, sizeof(double));

}