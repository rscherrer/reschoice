#ifndef RESCHOICE_READER_HPP
#define RESCHOICE_READER_HPP

// This header contains the Reader class.

#include "checker.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cassert>
#include <functional>

class Reader {

public:

    // Constructor
    Reader(const std::string&);

    // Setters
    void open();
    void readline();
    void close();

    // Breaker
    void readerror() const;
    
    // Getters
    bool isopen() const { return file.is_open(); }
    bool iseof() { return file.peek() == std::ifstream::traits_type::eof(); }
    bool iseol() { return line.peek() == std::istringstream::traits_type::eof(); }
    bool isempty() const { return empty; }
    bool iscomment() const { return comment; }
    size_t getcount() const { return count; }
    std::string getfilename() const { return filename; }
    std::string getline() const { return line.str(); }
    std::string getname() const { return name; }

    // Function to read a single value
    template <typename T> 
    void readvalue(
        T &value, 
        const std::function<std::string(const T&)> &check = nullptr
    ) {

        // value: variable to read into
        // check: function used to check the value
    
        // Read value in
        read(value, check);
    
        // Check that we have reached the end of the line
        if (!iseol())
            throw std::runtime_error(errorTooManyValues());
        
    }

    // Function to read a vector of values
    template <typename T> 
    void readvalues(
        std::vector<T> &values, 
        const size_t &n, 
        const std::function<std::string(const T&)> &check = nullptr, 
        const std::function<std::string(const std::vector<T>&)> &checks = nullptr
    ) {

        // values: vector to read into
        // n: number of values to read
        // check: function used to check individual values
        // checks: function used to check the vector of values

        // Check
        assert(n != 0);
    
        // Resize
        values.clear();
        values.reserve(n);
    
        // Counter
        size_t i = 0u;
    
        // While we have not reached the end of the line...
        while (!iseol()) {
    
            // If too many values...
            if (i == n) 
                throw std::runtime_error(errorTooManyValues());
            
            // Prepare to store the value
            T value;
    
            // Read the value
            read(value, check);
    
            // Add to the vector
            values.push_back(value);
    
            // Increment value counter
            ++i;
    
        }
    
        // If too few values...
        if (i != n) 
            throw std::runtime_error(errorTooFewValues());
        
        // Check
        assert(i == n);
        assert(values.size() == n);

        // Check validity (vector level)
        std::string error = checks ? checks(values) : "";

        // If error, throw
        checkerror(error);
    
    }

private:

    // File members
    std::string filename;
    std::ifstream file;
    
    // Line counter
    size_t count;

    // Line members
    bool empty;
    bool comment;
    std::istringstream line;
    std::string name;

    // Private setters
    void reset();
    bool readnext(std::istringstream&, std::string&);

    // Error messages
    std::string errorOpenFile() const;
    std::string errorEmptyFile() const;
    std::string errorReadName() const;
    std::string errorNoValue() const;
    std::string errorReadValue() const;
    std::string errorParseValue() const;
    std::string errorTooManyValues() const;
    std::string errorTooFewValues() const;
    std::string errorInvalidParameter() const;

    // Validity errors
    void checkerror(const std::string&) const;

    // // Function to read a value from the current line
    template <typename T> 
    void read(
        T &value, 
        const std::function<std::string(const T&)> &check = nullptr
    ) {

        // value: variable to read into
        // check: function used to check the value

        // Temporary receptacle
        std::string temp;

        // Make sure the next value can be read
        if (!readnext(line, temp)) 
            throw std::runtime_error(errorReadValue());
            
        // Prepare to store the value
        std::istringstream stream(temp);

        // Prepare to capture leftover characters 
        char leftover;

        // Read the value and check
        if (!(stream >> value) || (stream >> leftover))
            throw std::runtime_error(errorParseValue());

        // Check validity
        std::string error = check ? check(value) : "";

        // If error, throw
        checkerror(error);

    }
};

#endif