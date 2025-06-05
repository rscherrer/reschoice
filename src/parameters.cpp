// This script contains all the functions of the Parameters structure.

#include "parameters.hpp"

// Create a default seed based on clock
size_t clockseed() {

    // Use the current time to generate a seed
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    // Convert the seed to integer and return
    return static_cast<size_t>(seed);
}

Parameters::Parameters(const std::string &filename) :
    popsize(10u),
    xstart(0.0),
    tradeoff(1.0),
    alpha(0.0),
    beta(1.0),
    delta(1.0),
    hsymmetry(1.0),
    nrounds(10u),
    mutrate(0.0001),
    mutsdev(0.02),
    dispersal(0.1),
    tend(100u),
    tsave(20u),
    seed(clockseed()),
    savepars(true),
    savedat(false),
    savelog(false),
    verbose(false),
    choose(false),
    memsave(1.0)
{
    
    // filename: optional parameter input file

    // Read from file if needed
    if (filename != "") read(filename);

}

// Check that the parameter values are valid
void Parameters::check() const
{
    std::string msg = "No error detected";

    // Check the validity of the supplied values
    assert(popsize > 0u);
    assert(tradeoff >= 0.0);
    assert(alpha >= 0.0 && alpha <= 1.0);
    assert(beta >= 0.0 && beta <= 1.0);
    assert(delta >= 0.0);
    assert(hsymmetry >= 0.0 && hsymmetry <= 1.0);
    assert(nrounds > 0u);
    assert(mutrate >= 0.0 && mutrate <= 1.0);
    assert(mutsdev >= 0.0);
    assert(dispersal >= 0.0 && dispersal <= 1.0);
    assert(tend > 0u);
    assert(tsave > 0u);
    assert(memsave >= 0.0);
    
}

// Read parameters from a file
void Parameters::read(const std::string &filename)
{

    // filename: name of the file to read parameters from

    // Create a reader
    Reader reader(filename);

    // Open it
    reader.open();

    // For each line in the file...
    while (!reader.iseof()) {

        // Read a line
        reader.readline();

        // Skip empty line
        if (reader.isempty()) continue;

        // Skip if comment line
        if (reader.iscomment()) continue;

        // Check
        assert(!reader.isempty());
        assert(!reader.iscomment());
        
        // Current parameter name 
        std::string name = reader.getname();

        // Read the parameter value(s)
        if (name == "popsize") reader.readvalue<size_t>(popsize, chk::strictpos<size_t>);
        else if (name == "xstart") reader.readvalue<double>(xstart);
        else if (name == "tradeoff") reader.readvalue<double>(tradeoff, chk::positive<double>);
        else if (name == "alpha") reader.readvalue<double>(alpha, chk::proportion<double>);
        else if (name == "beta") reader.readvalue<double>(beta, chk::proportion<double>);
        else if (name == "delta") reader.readvalue<double>(delta, chk::positive<double>);
        else if (name == "hsymmetry") reader.readvalue<double>(hsymmetry, chk::proportion<double>);
        else if (name == "nrounds") reader.readvalue<size_t>(nrounds, chk::strictpos<size_t>);
        else if (name == "mutrate") reader.readvalue<double>(mutrate, chk::proportion<double>);
        else if (name == "mutsdev") reader.readvalue<double>(mutsdev, chk::positive<double>);
        else if (name == "dispersal") reader.readvalue<double>(dispersal, chk::proportion<double>);
        else if (name == "tend") reader.readvalue<size_t>(tend, chk::strictpos<size_t>);
        else if (name == "tsave") reader.readvalue<size_t>(tsave, chk::strictpos<size_t>);
        else if (name == "seed") reader.readvalue<size_t>(seed);
        else if (name == "savepars") reader.readvalue<bool>(savepars);
        else if (name == "savedat") reader.readvalue<bool>(savedat);
        else if (name == "savelog") reader.readvalue<bool>(savelog);
        else if (name == "verbose") reader.readvalue<bool>(verbose);
        else if (name == "choose") reader.readvalue<bool>(choose);
        else if (name == "memsave") reader.readvalue<double>(memsave, chk::enoughmb<double>);
        else
            reader.readerror();

    }

    // Close the file
    reader.close();

    // Check
    check();

}

// Save parameters to a file
void Parameters::save(const std::string &filename) const
{

    // filename: name of the file to save into

    // Create output file stream
    std::ofstream file(filename);
    
    // Check if the file is open
    if (!file.is_open()) 
        throw std::runtime_error("Unable to open file " + filename);

    // Write parameters to the file
    file << "popsize " << popsize << '\n';
    file << "xstart " << xstart << '\n';
    file << "tradeoff " << tradeoff << '\n';
    file << "alpha " << alpha << '\n';
    file << "beta " << beta << '\n';
    file << "delta " << delta << '\n';
    file << "hsymmetry " << hsymmetry << '\n';
    file << "nrounds " << nrounds << '\n';
    file << "mutrate " << mutrate << '\n';
    file << "mutsdev " << mutsdev << '\n';
    file << "dispersal " << dispersal << '\n';
    file << "tend " << tend << '\n';
    file << "tsave " << tsave << '\n';
    file << "seed " << seed << '\n';
    file << "savepars " << savepars << '\n';
    file << "savedat " << savedat << '\n';
    file << "savelog " << savelog << '\n';
    file << "verbose " << verbose << '\n';
    file << "choose " << choose << '\n';
    file << "memsave " << memsave << '\n';

    // Close the file
    file.close();

}