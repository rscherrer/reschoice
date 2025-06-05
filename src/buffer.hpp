#ifndef RESCHOICE_BUFFER_HPP
#define RESCHOICE_BUFFER_HPP

// This is the header for the Buffer class, which stores data in temporary containers
// before flusing it into some output file. One instance of this object has actually two
// internal containers, a head buffer and a tail buffer. The data are first saved in the 
// head buffer, then its content is moved to the tail buffer by swapping the two containers,
// and from there the content is flushed to file, and the tail buffer is cleared. In the
// meantime, the previous tail buffer has already been emptied and is now the new head buffer,
// ready to take in new values. This approach is to avoid too much waiting for I/O operations.  

#include <vector>
#include <fstream>
#include <memory>
#include <string>
#include <cassert>
#include <algorithm>

class Buffer {

public:

    // Constructor
    Buffer(const size_t&, const std::string&);

    // Setters
    void open();
    void save(const double&);
    void close();

    // Function to return the last value stored
    double last() const { return head->back(); };

    // Function to return buffer capacity
    size_t capacity() const { return head->capacity(); };

    // Function to return the number of values stored 
    size_t size() const { return head->size(); };

    // Function to tell if the file is open
    bool isopen() const { return file.is_open(); };

private:

    // Name of the file
    std::string filename;

    // Maximum possible size of the buffer
    size_t maxsize;

    // Smart pointers to data containers
    std::unique_ptr<std::vector<double> > head;
    std::unique_ptr<std::vector<double> > tail;

    // Smart pointer to an output file stream
    std::ofstream file;

    // Internal setters
    void flush();

};

#endif