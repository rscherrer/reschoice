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

// Function to save a value to output file
template <typename T> void stf::save(const T &x, std::shared_ptr<std::ofstream> &file) {

    const double x_ = static_cast<double>(x);
    file->write((char *) &x_, sizeof(double));

}