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

    void saveTime(const size_t&, std::shared_ptr<std::ofstream>&);
    void saveIndividualExpectedFitnessDifference(const double&, std::shared_ptr<std::ofstream>&);
    void saveIndividualChoice(const bool&, std::shared_ptr<std::ofstream>&);
    void saveResourceCensusFile(const size_t&, const size_t&, std::shared_ptr<std::ofstream>&);
    void saveResourceMeanTraitValue(const double&, const double&, std::shared_ptr<std::ofstream>&);
    void saveIndividualRealizedFitness(const double&, std::shared_ptr<std::ofstream>&);
    void saveIndividualHabitat(const bool&, std::shared_ptr<std::ofstream>&);
    void saveIndividualTraitValue(const double&, std::shared_ptr<std::ofstream>&);
    void saveIndividualTotalFitness(const double&, std::shared_ptr<std::ofstream>&);

    void save(const double&, std::shared_ptr<std::ofstream>&);
    void save(const bool&, std::shared_ptr<std::ofstream>&);
    void save(const size_t&, std::shared_ptr<std::ofstream>&);

    template <typename T> void save(const T&, std::shared_ptr<std::ofstream>&);

}

#endif

