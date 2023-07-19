#ifndef RESCHOICE_PRINTER_HPP
#define RESCHOICE_PRINTER_HPP

// This is the header for the Printer class.

#include <fstream>
#include <vector>
#include <memory>
#include <cassert>

struct Printer 
{

    Printer(size_t);

    template <typename T> void save(const T&, const size_t&);
    void close();

    std::vector<std::string> fnames;
    std::vector<std::ofstream> streams;

};

// Function to save a value to output file (defined here because template function)
template <typename T> void Printer::save(const T &x, const size_t &i) {

    // Make sure we are not out of bounds
    assert(i < streams.size());

    // If the corresponding stream is open...
    if (streams[i].is_open()) {

        // Convert the value into a double
        const double x_ = static_cast<double>(x);

        // Write it to the file
        streams[i].write((char *) &x_, sizeof(double));

    }
}

#endif

