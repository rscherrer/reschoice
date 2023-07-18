#ifndef RESCHOICE_PRINTER_HPP
#define RESCHOICE_PRINTER_HPP

// This header contains a series of functions for saving output to files

#include <fstream>
#include <vector>
#include <memory>

namespace stf
{

    void open(std::vector<std::shared_ptr<std::ofstream> >&, const std::vector<std::string>&);
    void close(std::vector<std::shared_ptr<std::ofstream> >&);
    void check(const std::vector<std::string>&, const std::vector<std::string>&);

    template <typename T> void save(const T&, std::shared_ptr<std::ofstream>&);

}

#endif

