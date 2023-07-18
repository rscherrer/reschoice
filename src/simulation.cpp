// This is the main simulation script. The core of the program has been
// encapsulated into a function called simulate(), so its proper behavior
// can be tested in different use cases in our tests.

#include "simulation.hpp"

// Lambda for removing dead individuals
auto burry = [](Individual ind) -> bool
{
    return !ind.isAlive();
};

int simulate(const std::vector<std::string> &args) {

	try
    {

		// Create a default parameter set
		Parameters pars;

		// Error if two many arguments
        if (args.size() > 2u) throw std::runtime_error("Two many arguments provided.");

		// Read parameters from a file if supplied
        if (args.size() == 2u) pars.read(args[1u]);

		// Save parameters if necessary
        if (pars.savepars) pars.save();

		// Redirect output to log file if needed
        if (pars.savelog) pars.savelog = std::freopen("log.txt", "w", stdout);

		// Create a vector of output file streams (using smart pointers)
        std::vector<std::shared_ptr<std::ofstream> > outfiles;

		// What variables do we save in this study?
		// - Time steps
		// - Individual habitat at each time step
		// - Individual ecotype at each time step (whether trait value is above or below population average)
		// - Individual trait value at each time step
		// - Individual total fitness at each time step
		// - Individual choice at each feeding round at each time step
		// - Individual position in the queue (rank) at each feeding round at each time step
		// - Individual realized fitness at each feeding round at each time step
		// - Individual expected fitness difference at each feeding round at each time step
		// - Number of individuals in each habitat at each time step
		// - Mean trait value in each habitat at each time step
		// - Number of individuals on each resource in each habitat at each feeding round at each time step
		// - Mean trait value on each resource in each habitat at each feeding round at each time step
		// - A statistic for ecological isolation between ecotypes at each time step
		// - A statistic for spatial isolation between ecotypes at each time step

		// Which variables to save
        std::vector<std::string> filenames = { 
			
			"time", "individualHabitat", "individualTraitValue", "individualEcotype", 
			"individualTotalFitness", "individualChoice", "individualRank", "individualRealizedFitness",
			"individualExpectedFitnessDifference", "habitatCensus", "habitatMeanTraitValue",
			"resourceCensus", "resourceMeanTraitValue", "ecologicalIsolation", "spatialIsolation"
		
		};

		// Update the list of which variables to save if needed...
        if (pars.choose) {

            // Read file where those are provided
            std::ifstream infile("whattosave.txt");
            if (!infile.is_open())
                throw std::runtime_error("Could not read input file whattosave.txt");
            std::vector<std::string> newfilenames;
            std::string input;
            while (infile >> input) newfilenames.push_back(input);
            stf::check(newfilenames, filenames);
            filenames.reserve(newfilenames.size());
            filenames.resize(newfilenames.size());
            for (size_t f = 0u; f < newfilenames.size(); ++f)
                filenames[f] = newfilenames[f];

        }

		// Open the file streams
        stf::open(outfiles, filenames);

		// Set up flags for which data to save
        int timeFile(-1), individualHabitatFile(-1), individualTraitValueFile(-1),
		individualEcotypeFile(-1), individualTotalFitnessFile(-1), individualChoiceFile(-1), 
		individualRankFile(-1), individualRealizedFitnessFile(-1), individualExpectedFitnessDifference(-1),
		habitatCensusFile(-1), habitatMeanTraitValueFile(-1), resourceCensusFile(-1),
		resourceMeanTraitValueFile(-1), ecologicalIsolationFile(-1), spatialIsolationFile(-1);

        for (size_t f = 0u; f < filenames.size(); ++f) {

            const std::string filename = filenames[f];

            if (filename == "time") timeFile = f;
			else if (filename == "individualHabitat") individualHabitatFile = f;
			else if (filename == "individualTraitValue") individualTraitValueFile = f;
			else if (filename == "individualEcotype") individualEcotypeFile = f;
			else if (filename == "individualTotalFitness") individualTotalFitnessFile = f;
			else if (filename == "individualChoice") individualChoiceFile = f;
			else if (filename == "individualRank") individualRankFile = f;
			else if (filename == "individualRealizedFitness") individualRealizedFitnessFile = f;
			else if (filename == "individualExpectedFitnessDifference") individualExpectedFitnessDifference = f;
			else if (filename == "habitatCensus") habitatCensusFile = f;
			else if (filename == "habitatMeanTraitValue") habitatMeanTraitValueFile = f;
			else if (filename == "resourceCensus") resourceCensusFile = f;
			else if (filename == "resourceMeanTraitValue") resourceMeanTraitValueFile = f;
			else if (filename == "ecologicalIsolation") ecologicalIsolationFile = f;
			else if (filename == "spatialIsolation") spatialIsolationFile = f;
            else throw std::runtime_error("Invalid output requested in whattosave.txt");

        }

		// Distribution of mutational deviations (set up here for speed)
		auto sampleMutation = rnd::normal(0.0, pars.mutsdev);

		// Create a population of individuals
		std::vector<Individual> pop(pars.popsize, { pars.tradeoff });

		// Make sure the container can contain twice the population size
		pop.reserve(2u * pop.size());

		// Create a matrix of resource concentrations
		std::vector<std::vector<double> > resources({{0.0, 0.0}, {0.0, 0.0}});
		for (size_t i = 0u; i < 2u; ++i)
			for (size_t j = 0u; j < 2u; ++j)
				resources[i][j] = i == pars.resource * (j ? 1.0 : pars.hsymmetry);

		// Screen output
		std::cout << "Simulation started.\n";

		// Loop through time steps
		for (size_t t = 0u; t <= pars.tend; ++t) {

			// Verbose if needed
			if (pars.talkative) std::cout << "t = " << t << '\n';

			// Flag to know if it is time to save some data
            const bool timeToSave = t % pars.tsave == 0u;

			// Save time if needed
			if (timeToSave && timeFile >= 0) 
				stf::save(t, outfiles[timeFile]);

			// There are multiple feeding rounds.
			// Every feeding round, individuals are taken in random order.
			// Eech individual assesses its expected fitness if feeding on each resource.
			// That expected fitness depends on the amount of resources present, the discovery rate, 
			// its own affinity for that resource and the cumulative affinity of all individuals
			// feeding on that resource (i.e. having chosen that resource).
			// Of those, the amount of resource and the discovery rate remain constant (resources
			// are replenished every round and do not go down until everyone has made a choice),
			// and the feeding efficiency only depends on the individual's trait value x.
			// The cumulative affinity does change as we loop through individuals within one round
			// though. Every time an individual makes a choice, that value changes, so we need
			// to keep track of it. Once all individuals have chosen, the resources is split
			// (well, only the amount of resources that was discovered). And it is split proportionately
			// to the realized fitness of the individuals, which uses the same formula as the
			// expected one but now with sums of feeding efficiencies being computed once everybodyvalues
			// throughout all feeding rounds.

			// Initialize a vector of fitnesses
			std::vector<double> fitnesses(pop.size());

			// Initialize a vector of consecutive indices
			std::vector<size_t> indices(pop.size());
			std::iota(indices.begin(), indices.end(), 0u);

			// For each feeding round...
			for (size_t j = 0u; j < pars.nrounds; ++j) {

				// Shuffle indices to take individuals in random order
				std::shuffle(indices.begin(), indices.end(), rnd::rng);

				// Initialize cumulative feeding efficiencies in each habitat on each resource
				std::vector<std::vector<double> > sumeffs({{0.0, 0.0}, {0.0, 0.0}});

				// Initialize other useful containers
				std::vector<std::vector<size_t> > n({{0u, 0u}, {0u, 0u}});
				std::vector<std::vector<double> > sumx({{0.0, 0.0}, {0.0, 0.0}}); 

				// For each individual...
				for (size_t i = 0; i < pop.size(); ++i) {

					// Respect random order
					const size_t ii = indices[i];

					// Assign a rank in the queue to the individual
					pop[ii].setRank(i);

					// Read individual properties
					const double x = pop[ii].getX();
					const bool habitat = pop[ii].getHabitat();

					// Get feeding efficiency on each resource
					const std::vector<double> effs({ pop[ii].getEff1(), pop[ii].getEff2() });

					// Compute expected fitness on each resource in the individual's habitat
					const double fit1 = resources[habitat][0u] * effs[0u] * (sumeffs[habitat][0u] + 1.0 / pars.delta - 1.0);
					const double fit2 = resources[habitat][1u] * effs[1u] * (sumeffs[habitat][1u] + 1.0 / pars.delta - 1.0);

					// Check that expected fitnesses are above zero
					assert(fit1 >= 0.0);
					assert(fit2 >= 0.0);

					// Record expected fitness difference and make the individual choose
					pop[ii].setDiff(fit2 - fit1);
					pop[ii].setChoice(fit2 > fit1, pars.beta);

					// Read the choice that was made
					const bool choice = pop[i].getChoice();

					// Update cumulative feeding efficiencies depending on what resource has been chosen, in that habitat
					sumeffs[habitat][choice] += effs[choice];

					// Update other important statistics
					++n[habitat][choice];
					sumx[habitat][choice] += x;

				}

				// Save the number of individuals feeding on each resource in each habitat if needed
				if (timeToSave && resourceCensusFile >= 0) {

					stf::save(n[0u][0u], outfiles[resourceCensusFile]);
					stf::save(n[0u][1u], outfiles[resourceCensusFile]);
					stf::save(n[1u][0u], outfiles[resourceCensusFile]);
					stf::save(n[1u][1u], outfiles[resourceCensusFile]);

				}

				// Save the mean trait value of individuals feeding on each resource in each habitat if needed
				if (timeToSave && resourceMeanTraitValueFile >= 0) {

					stf::save(n[0u][0u] ? sumx[0u][0u] / n[0u][0u] : 0.0, outfiles[resourceMeanTraitValueFile]);
					stf::save(n[0u][1u] ? sumx[0u][1u] / n[0u][1u] : 0.0, outfiles[resourceMeanTraitValueFile]);
					stf::save(n[1u][0u] ? sumx[1u][0u] / n[1u][0u] : 0.0, outfiles[resourceMeanTraitValueFile]);
					stf::save(n[1u][1u] ? sumx[1u][1u] / n[1u][1u] : 0.0, outfiles[resourceMeanTraitValueFile]);

				}

				// For each individual...
				for (size_t i = 0u; i < pop.size(); ++i) {

					// Read relevant individual properties
					const bool choice = pop[i].getChoice();
					const bool habitat = pop[i].getHabitat();
					const bool diff = pop[i].getDiff();
					const size_t rank = pop[i].getRank();

					// Corresponding feeding efficiency
					const double eff = choice ? pop[i].getEff2() : pop[i].getEff1();

					// Compute realized fitness on the chosen resource
					const double fit = resources[habitat][choice] * eff * (sumeffs[habitat][choice] + 1.0 / pars.delta - 1.0);

					// Check that the fitness is above zero
					assert(fit >= 0.0);

					// Add obtained food to the vector of fitnesses
					fitnesses[i] += fit;

					// Save individual expected fitness difference if needed
					if (timeToSave && individualExpectedFitnessDifference >= 0) 
						stf::save(diff, outfiles[individualExpectedFitnessDifference]);

					// Save individual choice if needed
					if (timeToSave && individualChoiceFile >= 0)
						stf::save(choice, outfiles[individualChoiceFile]);

					// Save individual realized fitness if needed
					if (timeToSave && individualRealizedFitnessFile >= 0) 
						stf::save(fit, outfiles[individualRealizedFitnessFile]);

					// Save individual rank in the queue if needed
					if (timeToSave && individualRankFile >= 0)
						stf::save(rank, outfiles[individualRankFile]);

					// Set individual ecotype relative to population average while we are looping through individuals
					if (!j) pop[i].setEcotype((sumx[0u][0u] + sumx[0u][1u] + sumx[1u][0u] + sumx[1u][1u]) / pop.size());

				}
			}

			// Create a distribution to sample parents from proportionately to fitness
			auto sampleParent = rnd::discrete(fitnesses.begin(), fitnesses.end());

			// Initialize useful containers for habitat- and ecotype-specific statistics
			std::vector<std::vector<size_t> > n({{0u, 0u}, {0u, 0u}});
			std::vector<std::vector<double> > sumx({{0.0, 0.0}, {0.0, 0.0}});
			std::vector<std::vector<double> > ssqx({{0.0, 0.0}, {0.0, 0.0}});

			// For each individual to be born...
			for (size_t i = 0u; i < pars.popsize; ++i) {

				// Sample parent of the current offspring (with replacement)
				const size_t j = sampleParent(rnd::rng);

				// Add offspring to the population by cloning
				pop.push_back(pop[j]);

				// Make sure it is alive (the adult may have been killed already)
				pop.back().isBorn(); 

				// Mutate offspring if needed
				if (rnd::bernoulli(pars.mutrate)(rnd::rng)) 
					pop.back().mutate(sampleMutation(rnd::rng), pars.tradeoff);

				// The offspring has a chance to disperse
				if (rnd::bernoulli(pars.dispersal)(rnd::rng))
					pop.back().disperse();

				// Kill an adult corresponding the the index of the current offspring
				pop[i].kill();

				// Get relevant individual metrics for that adult
				const double x = pop[i].getX();
				const bool habitat = pop[i].getHabitat();
				const bool ecotype = pop[i].getEcotype();

				// Update habitat- and ecotype-specific statistics
				++n[habitat][ecotype];
				sumx[habitat][ecotype] += x;
				ssqx[habitat][ecotype] += utl::sqr(x);

				// Save the habitat of that adult if needed
				if (timeToSave && individualHabitatFile >= 0)
					stf::save(habitat, outfiles[individualHabitatFile]);

				// Save the trait value of that adult if needed
				if (timeToSave && individualTraitValueFile >= 0) 
					stf::save(x, outfiles[individualTraitValueFile]);

				// Save the final fitness of that adult if needed
				if (timeToSave && individualTotalFitnessFile >= 0)
					stf::save(fitnesses[i], outfiles[individualTotalFitnessFile]);

				// Save the ecotype of that adult if needed
				if (timeToSave && individualEcotypeFile >= 0)
					stf::save(ecotype, outfiles[individualEcotypeFile]);

			}

			// Save the number of individuals in each habitat if needed
			if (timeToSave && habitatCensusFile >= 0) {

				stf::save(n[0u][0u] + n[0u][1u], outfiles[habitatCensusFile]);
				stf::save(n[1u][0u] + n[1u][1u], outfiles[habitatCensusFile]);

			}

			// Save the mean trait value in each habitat if needed
			if (timeToSave && habitatMeanTraitValueFile >= 0) {

				const size_t n1 = n[0u][0u] + n[0u][1u];
				const size_t n2 = n[1u][0u] + n[1u][1u];

				stf::save(n1 ? (sumx[0u][0u] + sumx[0u][1u]) / n1 : 0.0, outfiles[habitatMeanTraitValueFile]);
				stf::save(n2 ? (sumx[1u][0u] + sumx[1u][1u]) / n2 : 0.0, outfiles[habitatMeanTraitValueFile]);

			}

			// Save ecological isolation if needed
			if (timeToSave && ecologicalIsolationFile >= 0) {

				// Compute within-ecotype and whole-population variances
				const size_t n1 = n[0u][0u] + n[1u][0u];
				const size_t n2 = n[0u][1u] + n[1u][1u];
				const double sumx1 = sumx[0u][0u] + sumx[1u][0u];
				const double sumx2 = sumx[0u][1u] + sumx[1u][1u];
				const double ssqx1 = ssqx[0u][0u] + ssqx[1u][0u];
				const double ssqx2 = ssqx[0u][1u] + ssqx[1u][1u];
				const double varx1 = n1 ? ssqx1 / n1 - utl::sqr(sumx1 / n1) : 0.0;
				const double varx2 = n2 ? ssqx2 / n2 - utl::sqr(sumx2 / n2) : 0.0;
				const double varx0 = (ssqx1 + ssqx2) / pop.size() - utl::sqr((sumx1 + sumx2) / pop.size());

				// Make sure the variances are positive
				assert(varx1 >= 0.0);
				assert(varx2 >= 0.0);
				assert(varx0 >= 0.0);

				// Compute ecological isolation between ecotypes
				const double EI = varx0 ? 1.0 - (n1 * varx1 + n2 * varx2) / (pop.size() * varx0) : 0.0;

				// Make sure ecological isolation is between zero and one
				assert(EI >= 0.0 && EI <= 1.0);

				// Save it
				stf::save(EI, outfiles[ecologicalIsolationFile]);

			}

			// Save spatial isolation if needed
			if (timeToSave && spatialIsolationFile >= 0) {

				// Different components of the statistic
				const size_t n11 = n[0u][0u];
				const size_t n12 = n[0u][1u];
				const size_t n21 = n[1u][0u];
				const size_t n22 = n[1u][1u];
				const size_t n10 = n11 + n12;
				const size_t n20 = n21 + n22;
				const size_t n01 = n11 + n21;
				const size_t n02 = n12 + n22;

				// Compute the product that will go in the denominator
				const double prod = n10 * n20 * n01 * n02;

				// Make sure it is positive
				assert(prod >= 0.0);

				// Compute spatial isolation
				const double SI = prod ? fabs((n11 * n22 - n12 * n21) / sqrt(prod)) : 0.0;

				// Save it
				stf::save(SI, outfiles[spatialIsolationFile]);

			}

			// Remove dead individuals
            auto it = std::remove_if(pop.begin(), pop.end(), burry);
            pop.erase(it, pop.end());
            pop.shrink_to_fit();

			// Make sure population size has not changed
			assert(pop.size() == pars.popsize);

		}

		// Screen output
		std::cout << "Simulation ended.\n";

		// Close the log file if needed
		if (pars.savelog) std::fclose(stdout);

		// Close output file streams
        stf::close(outfiles);

        return 0;

    }
    catch (const std::exception& err)
    {
        std::cerr << "Exception: " << err.what() << '\n';
    }
    catch (const char* err)
    {
        std::cerr << "Exception: " << err << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown Exception\n";
    }
    return 1;

}
