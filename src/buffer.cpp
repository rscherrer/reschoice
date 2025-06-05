// This script contains member functions for the Buffer class.

#include "buffer.hpp"

// Constructor
Buffer::Buffer(const size_t &n, const std::string &name) :
    filename(name),
    maxsize(n),
    head(std::make_unique<std::vector<double> >()),
    tail(std::make_unique<std::vector<double> >()),
    file(std::ofstream())
{

    // n: size of the buffer
    // name: name of the ouput file 

    // Reserve space
    head->reserve(maxsize);
    tail->reserve(maxsize);

    // Check that the containers have reserved space
    assert(head->capacity() == maxsize);
    assert(tail->capacity() == maxsize);

    // Check that the containers are empty
    assert(head->empty());
    assert(tail->empty());

}

// Function to open the file
void Buffer::open() {

    // Open the file
    file.open(filename.c_str(), std::ios::binary);

    // Check that the file is open
    if (!file.is_open()) 
        throw std::runtime_error("Unable to open file " + filename);
    
}

// Function to store a new value
void Buffer::save(const double &x) {

    // x: the value to store

    // Check that we are below storage capacity
    assert(head->size() < maxsize);

    // Add the value to the active container
    head->push_back(x);

    // If the maximum allowed capacity is reached...
    if (head->size() == maxsize) flush();

}

// Function to write all the content of the buffer to file
void Buffer::flush() {

    // Make sure the file is open
    assert(file.is_open());

    // Swap the head and tail of the buffer
    std::swap(head, tail);

    // Write every stored value to file
    file.write((char *) tail->data(), tail->size() * sizeof(double));

    // Empty the tail
    tail->clear();

    // Make sure the buffer is empty
    assert(head->empty());
    assert(tail->empty());

}

// Function to close the output data file
void Buffer::close() {

    // Check that the file is open
    assert(file.is_open());

    // Flush whatever is left
    flush();

    // Close the file
    file.close();

    // Check that the file is closed
    assert(!file.is_open());

}