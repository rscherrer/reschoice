// This script contains function definitions of the Printer class

#include "printer.hpp"

// Function to convert computer memory in number of values
size_t prt::memtosize(const double &m, const double &u) {

    // m: memory use
    // u: the unit (e.g. 1 million for megabyte)

    // Check
    assert(m >= 0.0);
    assert(u >= 0.0);

    // How many values can be stored at most?
    return std::floor(m * u / sizeof(double));

}

// Constructor
Printer::Printer(const std::vector<std::string> &names, const double &memsave) :
    memory(prt::memtosize(memsave, 1E6)),
    outputs(names),
    valids(names),
    buffers(std::unordered_map<std::string, std::optional<Buffer> >())
{

    // names: names of the output variables
    // memsave: memory use (in MB)

    // Check
    assert(memory > 0u);

}

// Search for a string in a vector of strings
bool in(const std::string &x, const std::vector<std::string> &v) {

    // x: the string to search for
    // v: the vector to search in

    // Check
    assert(!v.empty());

    // For each element in the vector...
    for (const std::string &s : v) {

        // If the element is found, return true
        if (x == s) return true;

    }

    // If the element is not found, return false
    return false;

}

// Function to read a list of user defined outputs
void Printer::read(const std::string &filename) {

    // filename: the name of the file to read

    // Clear the list of outputs
    outputs.clear();

    // Open the file
    std::ifstream file(filename.c_str());

    // Check that the file is open
    if (!file.is_open())
        throw std::runtime_error("Unable to open file " + filename);

    // Prepare to read in requested outputs
    std::string input;

    // For each entry...
    while (file >> input) {

        // Check that the requested output is valid
        if (!in(input, valids))
            throw std::runtime_error("Invalid output requested in " + filename + ": " + input);

        // Save the requested output
        outputs.push_back(input); 

    }

    // Close the file
    file.close();

    // Resize
    outputs.shrink_to_fit();

}

// Function to open buffers
void Printer::open() {

    // Check
    assert(!outputs.empty());

    // For each output...
    for (auto &name : outputs) {

        // Set up a buffer
        buffers.emplace(name, Buffer(memory, name + ".dat"));

        // Find the buffer
        auto it = buffers.find(name);
        
        // Check
        assert(it != buffers.end());

        // Open the buffer
        it->second->open();

        // Check
        assert(it->second->isopen());

    }

    // Check
    assert(buffers.size() == outputs.size());

}

// Function to tell if a buffer exists
bool Printer::exists(const std::string &name) {

    // name: name of the buffer to check

    // Find the buffer
    auto it = buffers.find(name);

    // Check if exists
    return it != buffers.end();

}

// Function to tell if a buffer is open
bool Printer::isopen(const std::string &name) {

    // name: name of the buffer to check

    // Find the buffer
    auto it = buffers.find(name);

    // Check
    assert(it != buffers.end());

    // Check if open
    return it->second->isopen();

}

// Function to get the capacity of a buffer
size_t Printer::capacity(const std::string &name) {

    // name: name of the buffer to check

    // Find the buffer
    auto it = buffers.find(name);
    
    // Check
    assert(it != buffers.end());

    // Get capacity
    return it->second->capacity();

}

// Function to save data into a buffer
void Printer::save(const std::string &name, const double &x) {

    // name: name of the buffer in which to save
    // x: value to save

    // Find the buffer
    auto it = buffers.find(name);

    // Make sure it exists
    if (it == buffers.end()) return;

    // Check
    assert(it->second->isopen());

    // Save if it does
    it->second->save(x);

}

// Function to close buffers
void Printer::close() {

    // Check
    assert(!outputs.empty());

    // For each buffer...
    for (auto &buffer : buffers) {

        // Close the buffer
        buffer.second->close();

    }
}

// Function to check if all buffers are open
bool Printer::ison() {

    // Early exit if needed
    if (buffers.empty()) return false;

    // Check
    assert(!buffers.empty());

    // For each buffer...
    for (auto &buffer : buffers) {

        // Exit if any is found close
        if (!buffer.second->isopen()) return false;

    }

    // Otherwise say yes
    return true;

}