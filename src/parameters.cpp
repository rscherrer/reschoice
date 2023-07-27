// This script contains all the functions of the Parameters structure.

#include "parameters.hpp"

Parameters::Parameters() :
    popsize(10u),
    xstart(0.0),
    tradeoff(1.0),
    beta(1.0),
    delta(1.0),
    hsymmetry(1.0),
    nrounds(10u),
    typeII(false),
    mutrate(0.0001),
    mutsdev(0.02),
    dispersal(0.1),
    tend(100u),
    tsave(20u),
    seed(makeDefaultSeed()),
    savepars(true),
    savelog(false),
    talkative(false),
    whattosave(1u)
{
    // Make sure parameter values make sense
    check();

    // Seed the random number generator
    rnd::rng.seed(seed);
}

// Create a default seed based on clock
size_t Parameters::makeDefaultSeed()
{
    return static_cast<size_t>(std::chrono::high_resolution_clock::now().
     time_since_epoch().count());
}

// Read parameters from a file
void Parameters::read(const std::string &filename)
{
    std::ifstream inputfile;
    inputfile.open(filename);
    if (!inputfile.is_open()) {
        std::string msg = "Unable to open parameter file ";
        throw std::runtime_error(msg + filename);
    }

    import(inputfile);
    inputfile.close();
}

// Import parameters from a file stream
void Parameters::import(std::ifstream &file)
{

    // Prepare to record input from file
    std::string input;

    // Read input from file...
    while (file >> input) {

        // And update the corresponding parameter value
        if (input == "popsize") file >> popsize;
        else if (input == "xstart") file >> xstart;
        else if (input == "tradeoff") file >> tradeoff;
        else if (input == "beta") file >> beta;
        else if (input == "delta") file >> delta;
        else if (input == "hsymmetry") file >> hsymmetry;
        else if (input == "nrounds") file >> nrounds;
        else if (input == "typeII") file >> typeII;
        else if (input == "mutrate") file >> mutrate;
        else if (input == "mutsdev") file >> mutsdev;
        else if (input == "dispersal") file >> dispersal;
        else if (input == "tend") file >> tend;
        else if (input == "tsave") file >> tsave;
        else if (input == "seed") file >> seed;
        else if (input == "savepars") file >> savepars;
        else if (input == "savelog") file >> savelog;
        else if (input == "talkative") file >> talkative;
        else if (input == "whattosave") file >> whattosave;
        else
            throw std::runtime_error("Invalid parameter name: " + input);

    }

    // Now update interactive parameters
    update();

    std::cout << "Parameters were read in succesfully.\n";

}

// Update the system after having read new parameters
void Parameters::update()
{
    rnd::rng.seed(seed);
    check();
}

// Check that the parameter values are valid
void Parameters::check() const
{
    std::string msg = "No error detected";

    // Check the validity of the supplied values
    if (popsize == 0u) throw std::runtime_error("Initial population size cannot be zero");
    if (tradeoff < 0.0) throw std::runtime_error("Trade-off cannot be negative");
    if (beta > 1.0 || beta < 0.0) throw std::runtime_error("Resource choice accuracy should be between zero and one");
    if (delta < 0.0) throw std::runtime_error("Resource discovery rate cannot be negative");
    if (hsymmetry < 0.0 || hsymmetry > 1.0) throw std::runtime_error("Habitat symmetry should be between zero and one");
    if (nrounds == 0u) throw std::runtime_error("There should be at least one feeding round");
    if (mutrate < 0.0 || mutrate > 1.0) throw std::runtime_error("Mutation rate should be between zero and one");
    if (mutsdev < 0.0) throw std::runtime_error("Mutational standard deviation cannot be negative");
    if (dispersal < 0.0 || dispersal > 1.0) throw std::runtime_error("Dispersal rate should be between zero and one");
    if (tend == 0u) throw std::runtime_error("Simulation time cannot be zero");
    if (tsave == 0u) throw std::runtime_error("Cannot save data every zero time point");
    
}

// Save parameters to a file
void Parameters::save() const
{
    const std::string filename = "paramlog.txt";
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Unable to open file " + filename);
    write(file);
    file.close();
}

// Write parameters to a file stream
void Parameters::write(std::ofstream &file) const
{

    // Write names and values separated by spaces
    file << "popsize " << popsize << '\n';
    file << "xstart " << xstart << '\n';
    file << "tradeoff " << tradeoff << '\n';
    file << "beta " << beta << '\n';
    file << "delta " << delta << '\n';
    file << "hsymmetry " << hsymmetry << '\n';
    file << "nrounds " << nrounds << '\n';
    file << "typeII " << typeII << '\n';
    file << "mutrate " << mutrate << '\n';
    file << "mutsdev " << mutsdev << '\n';
    file << "dispersal " << dispersal << '\n';
    file << "tend " << tend << '\n';
    file << "tsave " << tsave << '\n';
    file << "seed " << seed << '\n';
    file << "savepars " << savepars << '\n';
    file << "savelog " << savelog << '\n';
    file << "talkative " << talkative << '\n';
    file << "whattosave " << whattosave << '\n';

}
