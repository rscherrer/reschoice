// Source code of the Reader class.

#include "reader.hpp"

// Constructor
Reader::Reader(const std::string &filename) : 
    filename(filename),
    file(std::ifstream()),
    count(0u),
    empty(false),
    comment(false),
    line(std::istringstream()),
    name("")
{

    // filename: name of the file to read

}

// Error messages
std::string Reader::errorOpenFile() const { return "Unable to open file " + filename; }
std::string Reader::errorEmptyFile() const { return "File " + filename + " is empty"; }
std::string Reader::errorReadName() const { return "Could not read parameter name in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorNoValue() const { return "No value for parameter " + name + " in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorReadValue() const { return "Could not read value for parameter " + name + " in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorParseValue() const { return "Invalid value type for parameter " + name + " in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorTooManyValues() const { return "Too many values for parameter " + name + " in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorTooFewValues() const { return "Too few values for parameter " + name + " in line " + std::to_string(count) + " of file " + filename; }
std::string Reader::errorInvalidParameter() const { return "Invalid parameter: " + name + " in line " + std::to_string(count) + " of file " + filename; }

// Function to error on invalid parameter
void Reader::readerror() const {

    // Throw error
    throw std::runtime_error(errorInvalidParameter());

}

// Function to format error message
void Reader::checkerror(const std::string &error) const {

    // error: error message to format

    // Check if error is empty
    if (error.empty()) return;

    // Or format the error message
    std::string message = "Parameter " + name + " " + error + " in line " + std::to_string(count) + " of file " + filename;

    // And throw exception
    throw std::runtime_error(message);

}

// Function to open the file
void Reader::open() {

    // Open the file
    file.open(filename.c_str());

    // Check if the file is open
    if (!isopen())
        throw std::runtime_error(errorOpenFile());

    // Check if the file is empty
    if (iseof())
        throw std::runtime_error(errorEmptyFile());

    // Check
    assert(isopen());
    assert(!iseof());
    assert(count == 0u);

}

// Function to reset a line
void Reader::reset() {

    // Reset
    empty = false;
    comment = false;
    line.clear();
    line.str("");
    line.seekg(0, std::ios::beg);
    name.clear();

}

// Function to make sure the next thing can be read
bool Reader::readnext(std::istringstream &line, std::string &input) {

    // line: line to read from
    // input: string to read into

    // Read the parameter name
    line >> input;

    // Check if error
    bool error = line.fail();

    // If not...
    if (!error) {

        // For each character...
        for (char c : input) {

            // Make sure it is alphanumeric or a dot or a minus
            if (!std::isalnum(c) && c != '.' && c != '-') error = true;

            // Exit if needed
            if (error) break;

        }
    }

    // Return error code
    return !error;

}

// Function to read a line from the file
void Reader::readline() {

    // Check
    assert(isopen());
    assert(!iseof());

    // Reset
    reset();

    // Temporary container
    std::string temp;

    // Read the line
    std::getline(file, temp);

    // Check if the line is empty
    empty = temp.empty();

    // Check if the line is a comment
    comment = temp[0] == '#';

    // Convert the line into a stream
    line.str(temp);

    // Increment line count
    ++count;

    // If needed...
    if (empty || comment) return;

    // Error if needed
    if (!readnext(line, name))
        throw std::runtime_error(errorReadName());

    // Check that we are not at the end of the line
    if (iseol())
        throw std::runtime_error(errorNoValue());

}

// Function to close the input file
void Reader::close() {

    // Close
    file.close();

}