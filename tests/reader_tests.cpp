#define BOOST_TEST_DYNAMIC_LINK
#define BOOST_TEST_MODULE Main

// Here we test the parameter reading utilties

#include "testutils.hpp"
#include "../src/reader.hpp"
#include <boost/test/unit_test.hpp>

// Test that a reader initializes properly
BOOST_AUTO_TEST_CASE(readerInitilization) {

    // Write a parameter file
    tst::write("parameters.txt", "nloci 10\npopsize 10");

    // Create a reader
    Reader reader("parameters.txt");

    // Check elements
    BOOST_CHECK_EQUAL(reader.isopen(), false);
    BOOST_CHECK_EQUAL(reader.getcount(), 0u);
    BOOST_CHECK_EQUAL(reader.getfilename(), "parameters.txt");
    BOOST_CHECK_EQUAL(reader.getname(), "");
    BOOST_CHECK_EQUAL(reader.getline(), "");

    // Close the file
    reader.close();

    // Remove the file
    std::remove("parameters.txt");
 
}

// Test that a reader opens properly
BOOST_AUTO_TEST_CASE(readerOpen) {

    // Write a parameter file
    tst::write("parameters.txt", "nloci 10\npopsize 10");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Check elements
    BOOST_CHECK_EQUAL(reader.isopen(), true);
    BOOST_CHECK_EQUAL(reader.getcount(), 0u);
    BOOST_CHECK_EQUAL(reader.isempty(), false);
    BOOST_CHECK_EQUAL(reader.iscomment(), false);

    // Close the file
    reader.close();

    // Remove the file
    std::remove("parameters.txt");
 
}

// Test that a reader throws an error when opening a non-existent file
BOOST_AUTO_TEST_CASE(readerErrorFileNotFound) {

    // Create a reader
    Reader reader("nonexistent.txt");

    // Check that it throws an error
    tst::checkError([&]() { reader.open(); }, "Unable to open file nonexistent.txt");

}

// Test that a reader throws an error when reading an empty file
BOOST_AUTO_TEST_CASE(readerErrorEmptyFile) {

    // Write an empty parameter file
    tst::write("empty.txt", "");

    // Create a reader
    Reader reader("empty.txt");

    // Check that it throws an error
    tst::checkError([&]() { reader.open(); }, "File empty.txt is empty");

    // Remove the file
    std::remove("empty.txt");

}

// Test that a reader closes properly
BOOST_AUTO_TEST_CASE(readerClose) {

    // Write a parameter file
    tst::write("parameters.txt", "nloci 10\npopsize 10");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Make sure it is open
    BOOST_REQUIRE(reader.isopen());

    // Close the file
    reader.close();

    // Check elements
    BOOST_CHECK_EQUAL(reader.isopen(), false);

    // Remove the file
    std::remove("parameters.txt");
 
}

// Test that a reader reads a line properly
BOOST_AUTO_TEST_CASE(readerReadLine) {

    // Write a parameter file
    tst::write("parameters.txt", "nloci 10\npopsize 10");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Read the first line
    reader.readline();

    // Check elements
    BOOST_CHECK_EQUAL(reader.getcount(), 1u);
    BOOST_CHECK_EQUAL(reader.getline(), "nloci 10");
    BOOST_CHECK_EQUAL(reader.isempty(), false);
    BOOST_CHECK_EQUAL(reader.iscomment(), false);
    BOOST_CHECK_EQUAL(reader.getname(), "nloci");

    // We are not at the end of the file
    BOOST_CHECK_EQUAL(reader.iseof(), false); 

    // Read the second line
    reader.readline();

    // Check elements
    BOOST_CHECK_EQUAL(reader.getcount(), 2u);
    BOOST_CHECK_EQUAL(reader.getline(), "popsize 10");
    BOOST_CHECK_EQUAL(reader.isempty(), false);
    BOOST_CHECK_EQUAL(reader.iscomment(), false);
    BOOST_CHECK_EQUAL(reader.getname(), "popsize");

    // We are at the end of the file
    BOOST_CHECK_EQUAL(reader.iseof(), true);

    // Close the file
    reader.close();

    // Remove the file
    std::remove("parameters.txt");
}

// Test that an empty line can be detected
BOOST_AUTO_TEST_CASE(readerEmptyLine) {

    // Write a parameter file
    tst::write("parameters.txt", "nloci 10\n\npopsize 10");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Read the first line
    reader.readline();

    // Check elements
    BOOST_CHECK_EQUAL(reader.getcount(), 1u);
    BOOST_CHECK_EQUAL(reader.getline(), "nloci 10");
    BOOST_CHECK_EQUAL(reader.isempty(), false);
    BOOST_CHECK_EQUAL(reader.getname(), "nloci");

    // Read the second line
    reader.readline();

    // Check elements
    BOOST_CHECK_EQUAL(reader.getcount(), 2u);
    BOOST_CHECK_EQUAL(reader.getline(), "");
    BOOST_CHECK_EQUAL(reader.isempty(), true);
    BOOST_CHECK_EQUAL(reader.getname(), "");

    // Read the third line
    reader.readline();

    // Check elements 
    BOOST_CHECK_EQUAL(reader.getcount(), 3u);
    BOOST_CHECK_EQUAL(reader.getline(), "popsize 10");
    BOOST_CHECK_EQUAL(reader.isempty(), false);
    BOOST_CHECK_EQUAL(reader.getname(), "popsize");

}

// Test that a comment line can be detected
BOOST_AUTO_TEST_CASE(readerCommentLine) {

    // Write a parameter file
    tst::write("parameters.txt", "# This is a comment\npopsize 10");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Read the first line
    reader.readline();

    // Check elements
    BOOST_CHECK_EQUAL(reader.getcount(), 1u);
    BOOST_CHECK_EQUAL(reader.getline(), "# This is a comment");
    BOOST_CHECK_EQUAL(reader.isempty(), false);
    BOOST_CHECK_EQUAL(reader.iscomment(), true);
    BOOST_CHECK_EQUAL(reader.getname(), "");
    
    // Read the second line
    reader.readline();

    // Check elements
    BOOST_CHECK_EQUAL(reader.getcount(), 2u);
    BOOST_CHECK_EQUAL(reader.getline(), "popsize 10");
    BOOST_CHECK_EQUAL(reader.iscomment(), false);
    BOOST_CHECK_EQUAL(reader.isempty(), false);
    BOOST_CHECK_EQUAL(reader.getname(), "popsize");

}

// Test error when reading a parameter name fails
BOOST_AUTO_TEST_CASE(readerErrorReadName) {

    // Write a parameter file with a control character in parameter name
    tst::write("parameters.txt", "hel\x01lo 10\npopsize 10");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Check that it throws an error
    tst::checkError([&]() { reader.readline(); }, "Could not read parameter name in line 1 of file parameters.txt");

    // Close the file
    reader.close();

    // Remove the file
    std::remove("parameters.txt");
 
}

// Test error when value is missing
BOOST_AUTO_TEST_CASE(readerErrorNoValue) {

    // Write a parameter file with a missing value
    tst::write("parameters.txt", "nloci\npopsize 10");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Check that it throws an error
    tst::checkError([&]() { reader.readline(); }, "No value for parameter nloci in line 1 of file parameters.txt");

    // Close the file
    reader.close();

    // Remove the file
    std::remove("parameters.txt");
 
}

// Test that reading a single value works
BOOST_AUTO_TEST_CASE(readerReadValue) {

    // Write a parameter file
    tst::write("parameters.txt", "nloci 10\npopsize 10");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Read the first line
    reader.readline();

    // Read the value
    size_t nloci = 0u;
    reader.readvalue(nloci);

    // Check elements
    BOOST_CHECK_EQUAL(nloci, 10u);

    // Read the next line
    reader.readline();

    // Read the value
    size_t popsize = 0u;
    reader.readvalue(popsize);

    // Check
    BOOST_CHECK_EQUAL(popsize, 10u);

}

// Test that reading a single value fails if more are supplied
BOOST_AUTO_TEST_CASE(readerErrorNotJustOneValue) {

    // Write a parameter file
    tst::write("parameters.txt", "nloci 10 20\npopsize 10");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Read the first line
    reader.readline();

    // Check that it throws an error
    size_t nloci = 0u;
    tst::checkError([&]() { reader.readvalue(nloci); }, "Too many values for parameter nloci in line 1 of file parameters.txt");

    // Close the file
    reader.close();

    // Remove the file
    std::remove("parameters.txt");
 
}

// Test that fails when parameter value cannot be read
BOOST_AUTO_TEST_CASE(readerErrorReadValue) {

    // Write a parameter file with control character
    tst::write("parameters.txt", "nloci \x10\npopsize 10");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Read the first line
    reader.readline();

    // Read the value
    size_t nloci = 0u;

    // Check error
    tst::checkError([&]() { reader.readvalue(nloci); }, "Could not read value for parameter nloci in line 1 of file parameters.txt");

    // Close the file
    reader.close();

    // Remove the file
    std::remove("parameters.txt");

}

// Test that fails when parameter value cannot be parsed
BOOST_AUTO_TEST_CASE(readerErrorParseValue) {

    // Write a parameter file with a non-integer value
    tst::write("parameters1.txt", "nloci 12.5\npopsize 10");
    tst::write("parameters2.txt", "nloci hello\npopsize 10");

    // Create a reader
    Reader r1("parameters1.txt");
    Reader r2("parameters2.txt");

    // Open the file
    r1.open();
    r2.open();

    // Read the first line
    r1.readline();
    r2.readline();

    // Read the value
    size_t nloci = 0u;

    // Check error
    tst::checkError([&]() { r1.readvalue(nloci); }, "Invalid value type for parameter nloci in line 1 of file parameters1.txt");
    tst::checkError([&]() { r2.readvalue(nloci); }, "Invalid value type for parameter nloci in line 1 of file parameters2.txt");

    // Close the file
    r1.close();
    r2.close();

    // Remove the file
    std::remove("parameters1.txt");
    std::remove("parameters2.txt");

}

// Test that fails when values are invalid given provided criteria
BOOST_AUTO_TEST_CASE(readerErrorInvalidValue) {

    // Write a parameter file
    tst::write("parameters1.txt", "hello -1\nhello -1\nhello -1");
    tst::write("parameters2.txt", "hello 0\nhello 0\nhello 0\nhello 0");
    tst::write("parameters3.txt", "hello 1.5");
    tst::write("parameters4.txt", "hello 1001");
    tst::write("parameters5.txt", "hello 0.00000000000000001");

    // Note: We add multiple lines with the same parameters so we can call
    // the reader functions on the same parameter file several times in
    // a row to test various error messages.

    // Create a reader
    Reader r1("parameters1.txt");
    Reader r2("parameters2.txt");
    Reader r3("parameters3.txt");
    Reader r4("parameters4.txt");
    Reader r5("parameters5.txt");

    // Open the file
    r1.open();
    r2.open();
    r3.open();
    r4.open();
    r5.open();

    // Read the first line
    r1.readline();
    r2.readline();
    r3.readline();
    r4.readline();
    r5.readline();

    // Prepare to read
    size_t n = 0u;
    double x = 0.0;

    // Parameter file with a negative number
    tst::checkError([&]() { r1.readvalue<double>(x, chk::positive<double>); }, "Parameter hello must be positive in line 1 of file parameters1.txt");
    r1.readline();
    tst::checkError([&]() { r1.readvalue<double>(x, chk::strictpos<double>); }, "Parameter hello must be strictly positive in line 2 of file parameters1.txt");
    r1.readline();
    tst::checkError([&]() { r1.readvalue<double>(x, chk::proportion<double>); }, "Parameter hello must be between 0 and 1 in line 3 of file parameters1.txt");
    
    // Parameter file with a zero
    tst::checkError([&]() { r2.readvalue<double>(x, chk::strictpos<double>); }, "Parameter hello must be strictly positive in line 1 of file parameters2.txt");
    r2.readline();
    tst::checkError([&]() { r2.readvalue<size_t>(n, chk::strictpos<size_t>); }, "Parameter hello must be strictly positive in line 2 of file parameters2.txt");
    r2.readline();
    tst::checkError([&]() { r2.readvalue<size_t>(n, chk::onetothousand<size_t>); }, "Parameter hello must be between 1 and 1000 in line 3 of file parameters2.txt");
    r2.readline();
    tst::checkError([&]() { r2.readvalue<double>(x, chk::enoughmb<double>); }, "Parameter hello must be enough MB to store a double in line 4 of file parameters2.txt");

    // Other parameter files
    tst::checkError([&]() { r3.readvalue<double>(x, chk::proportion<double>); }, "Parameter hello must be between 0 and 1 in line 1 of file parameters3.txt");
    tst::checkError([&]() { r4.readvalue<size_t>(n, chk::onetothousand<size_t>); }, "Parameter hello must be between 1 and 1000 in line 1 of file parameters4.txt");
    tst::checkError([&]() { r5.readvalue<double>(x, chk::enoughmb<double>); }, "Parameter hello must be enough MB to store a double in line 1 of file parameters5.txt");

    // Close the file
    r1.close();
    r2.close();
    r3.close();
    r4.close();
    r5.close();

    // Remove the file
    std::remove("parameters1.txt");
    std::remove("parameters2.txt");
    std::remove("parameters3.txt");
    std::remove("parameters4.txt");
    std::remove("parameters5.txt");
 
}

// Test that reading a vector of values works
BOOST_AUTO_TEST_CASE(readerReadVector) {

    // Write a parameter file
    tst::write("parameters.txt", "nloci 10 20 30\npopsize -1 -2 -3");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Read the first line
    reader.readline();

    // Read the value
    std::vector<size_t> nloci;
    reader.readvalues(nloci, 3u);

    // Check elements
    BOOST_CHECK_EQUAL(nloci.size(), 3u);
    BOOST_CHECK_EQUAL(nloci[0u], 10u);
    BOOST_CHECK_EQUAL(nloci[1u], 20u);
    BOOST_CHECK_EQUAL(nloci[2u], 30u);

    // Read the next line
    reader.readline();

    // Read the value
    std::vector<double> popsize;
    reader.readvalues(popsize, 3u);

    // Check
    BOOST_CHECK_EQUAL(popsize.size(), 3u);
    BOOST_CHECK_EQUAL(popsize[0u], -1.0);
    BOOST_CHECK_EQUAL(popsize[1u], -2.0);
    BOOST_CHECK_EQUAL(popsize[2u], -3.0);

}

// Test that reading a vector of values fails if too many are supplied
BOOST_AUTO_TEST_CASE(readerErrorTooManyValues) {

    // Write a parameter file
    tst::write("parameters.txt", "nloci 10 20 30 40\npopsize -1 -2 -3");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Read the first line
    reader.readline();

    // Check that it throws an error
    std::vector<size_t> nloci;
    tst::checkError([&]() { reader.readvalues(nloci, 3u); }, "Too many values for parameter nloci in line 1 of file parameters.txt");

    // Close the file
    reader.close();

    // Remove the file
    std::remove("parameters.txt");
 
}

// Test that reading a vector of values fails if too few are supplied
BOOST_AUTO_TEST_CASE(readerErrorTooFewValues) {

    // Write a parameter file
    tst::write("parameters.txt", "nloci 10 20\npopsize -1 -2 -3");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Read the first line
    reader.readline();

    // Check that it throws an error
    std::vector<size_t> nloci;
    tst::checkError([&]() { reader.readvalues(nloci, 3u); }, "Too few values for parameter nloci in line 1 of file parameters.txt");

    // Close the file
    reader.close();

    // Remove the file
    std::remove("parameters.txt");
 
}

// Test that error when values are not in strict order
BOOST_AUTO_TEST_CASE(readerErrorInvalidOrder) {

    // Write a parameter file
    tst::write("parameters1.txt", "nloci 30 10 20\npopsize -1 -2 -3");
    tst::write("parameters2.txt", "nloci 1 1 3\npopsize -1 -2 -3");

    // Create a reader
    Reader r1("parameters1.txt");
    Reader r2("parameters2.txt");

    // Open the file
    r1.open();
    r2.open();

    // Read the first line
    r1.readline();
    r2.readline();

    // Prepare
    std::vector<size_t> nloci;

    // Check
    tst::checkError([&]() { r1.readvalues<size_t>(nloci, 3u, nullptr, chk::strictorder<size_t>); }, "Parameter nloci must be in strictly increasing order in line 1 of file parameters1.txt");
    tst::checkError([&]() { r2.readvalues<size_t>(nloci, 3u, nullptr, chk::strictorder<size_t>); }, "Parameter nloci must be in strictly increasing order in line 1 of file parameters2.txt");

    // Close the file
    r1.close();
    r2.close();

    // Remove the file
    std::remove("parameters1.txt");
    std::remove("parameters2.txt");
 
}

// Test of throwing an invalid parametr error
BOOST_AUTO_TEST_CASE(readerErrorInvalidParameter) {

    // Write a parameter file
    tst::write("parameters.txt", "nloci 10\npopsize 10");

    // Create a reader
    Reader reader("parameters.txt");

    // Open the file
    reader.open();

    // Read the first line
    reader.readline();

    // Check that it throws an error
    tst::checkError([&]() { reader.readerror(); }, "Invalid parameter: nloci in line 1 of file parameters.txt");

    // Close the file
    reader.close();

    // Remove the file
    std::remove("parameters.txt");
 
}  