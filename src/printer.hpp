#ifndef RESCHOICE_PRINTER_HPP
#define RESCHOICE_PRINTER_HPP

// This header is for the Printer class, which saves data to output files
// by means of buffers.

#include "buffer.hpp"

#include <unordered_map>
#include <optional>
#include <cmath>

namespace prt {

    // Utility functions
    size_t memtosize(const double&, const double&);

}

class Printer {

public:

    // Constructor
    Printer(const std::vector<std::string>&, const double& = 1.0);

    // Setters
    void read(const std::string&);
    void open();
    void close();

    // Buffer setters
    void save(const std::string&, const double&);

    // Getters
    bool ison();

    // Buffer getters
    bool exists(const std::string&);
    bool isopen(const std::string&);
    size_t capacity(const std::string&);

private:

    // Buffer size (in number of values)
    size_t memory;

    // Output variable names
    std::vector<std::string> outputs;

    // Valid names
    std::vector<std::string> valids;

    // Data buffers
    std::unordered_map<std::string, std::optional<Buffer> > buffers;

};

#endif

