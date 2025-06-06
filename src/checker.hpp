#ifndef RESCHOICE_CHECKER_HPP
#define RESCHOICE_CHECKER_HPP

// This is the header for the chk (checker) namespace.

#include <vector>
#include <string>

namespace chk {

    // Function to check that a value is a proportion
    template <typename T> 
    std::string proportion(const T &x) { 
        
        return x < 0.0 || x > 1.0 ? "must be between 0 and 1" : "";

    }

    // Function to check that a value is between 1 and 1000
    template <typename T>
    std::string onetothousand(const T &x) {

        return x <= 0u || x > 1000u ? "must be between 1 and 1000" : "";

    }

    // Function to check that a value is enough MB
    template <typename T>
    std::string enoughmb(const T &x) {
        
        return x * 1E6 < sizeof(double) ? "must be enough MB to store a double" : "";

    }

    // Function to check that a value is positive
    template <typename T>
    std::string positive(const T &x) {

        return x < 0.0 ? "must be positive" : "";

    }

    // Function to check that a value is strictly positive
    template <typename T>
    std::string strictpos(const T &x) {

        return x <= 0.0 ? "must be strictly positive" : "";

    }

    // Function to check that a vector is in order
    template <typename T> 
    std::string strictorder(const std::vector<T> &v) {

        // Early exit
        if (v.size() < 2u) return "";

        // Check if any two are not
        for (size_t i = 1u; i < v.size(); ++i)
            if (v[i] <= v[i - 1u])
                return "must be in strictly increasing order";

        return "";
        
    }
}

#endif