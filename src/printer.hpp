#ifndef RESCHOICE_PRINTER_HPP
#define RESCHOICE_PRINTER_HPP

// This is the header for the Printer class.

#include <fstream>
#include <vector>
#include <memory>
#include <cassert>

struct Printer 
{

    // Constructor
    Printer(size_t);

    // Functions
    template <typename T> void save(const T&, const size_t&);
    void flush();
    void close();

    // Member variables
    size_t size;                                    // amount of data saved (in bytes)
    std::vector<std::string> fnames;                // file names
    std::vector<std::vector<double> > buffers;      // buffers to store values for each file
    std::vector<std::ofstream> streams;             // streams to files where to write

};

// Function to store a value in memory (defined here because template function)
template <typename T> void Printer::save(const T &x, const size_t &i) {

    // x: the value to save
    // i: the index of the variable being saved

    // Make sure we are not out of bounds
    assert(i < streams.size());

    // Convert the value into a double
    const double x_ = static_cast<double>(x);

    // Store value in the corresponding buffer
    buffers[i].push_back(x_);

    // Track the amount of data saved
    size += sizeof(double);

}

#endif

