// This file contains function definitions for class Population.

#include "population.hpp"

// Constructor
Population::Population(const Parameters &pars) :
    individuals(std::make_unique<std::vector<Individual> >()),
    newborns(std::make_unique<std::vector<Individual> >()),
    popsize(pars.popsize),
    tradeoff(pars.tradeoff),
    alpha(pars.alpha),
    beta(pars.beta),
    delta(pars.delta),
    hsymmetry(pars.hsymmetry),
    nrounds(pars.nrounds),
    mutrate(pars.mutrate),
    mutsdev(pars.mutsdev),
    dispersal(pars.dispersal),
    tend(pars.tend),
    tsave(pars.tsave),
	verbose(pars.verbose),
    resources({{1.0, hsymmetry}, {hsymmetry, 1.0}}),
    sampleMutation(rnd::normal(0.0, mutsdev)),
    time(0u)
{

    // pars: parameter set

    // Check
    check();

    // Reserve space for the population
    individuals->reserve(popsize);
    newborns->reserve(0u);

    // Check
    assert(individuals->capacity() == popsize);
    assert(newborns->capacity() == 0u);

    // Fill the population with individuals
    for (size_t i = 0u; i < popsize; ++i) 
        individuals->emplace_back(Individual(pars.xstart, tradeoff));

    // Check
    assert(individuals->size() == popsize);
    assert(newborns->empty());

}

// Function to check parameter values
void Population::check() const {

    // Check that the parameter values are valid
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
    assert(time >= 0);

}

// Function to increment time
void Population::moveon() {

    // Update time
    ++time;

}

// Function to compute the amount of resource discovered by a group of feeders
double pop::discover(const double &rtotal, const double &delta, const double &sumeffs) {

	// rtotal: total amount of resource in the habitat
	// delta: resource discovery rate
	// sumeffs: cumulative consumption rate of the feeders

	// Check
	assert(rtotal >= 0.0);
	assert(delta >= 0.0);
	assert(sumeffs >= 0.0);

	// Early exit in case of zero (avoid unnecessary exponential)
	if (!sumeffs || !delta || !rtotal) return 0.0;

	// Compute the saturating curve
	return rtotal * (1.0 - exp(-delta * sumeffs));

}

// Function to compute estimated fitness of an individual
double pop::fitness(const double &rdiscov, const double &eff, const double &sumeffs, const size_t &n) {

	// rdiscov: amount of resource discovered
	// eff: consumption efficiency of the focal individual
	// sumeffs: cumulative consumption rate of all the feeders (incl. focal individual)
	// n: number of feeders (only used if sumeffs is zero)

	// Check
	assert(rdiscov >= 0.0);
	assert(eff >= 0.0);
	assert(sumeffs >= eff);

	// Special case if cumulative consumption rate (denominator) is zero
	if (!sumeffs) return n ? rdiscov * 1.0 / n : 0.0;

	// Note: if there are feeders but all with consumption rate of zero, they
	// split the resource equally. Fitness is only zero if the reason why the
	// cumulative rate is zero is that there are no feeders.

	// Compute fitness
	return rdiscov * eff / sumeffs;

}

// Function to compute the ecological isolation statistic
double stat::ei(
	
	const std::vector<std::vector<size_t> > &n,
	const std::vector<std::vector<double> > &sumx,
	const std::vector<std::vector<double> > &ssqx
	
) {

	// Useful counts
	const size_t n0 = n[0u][0u] + n[0u][1u] + n[1u][0u] + n[1u][1u];
	const size_t n1 = n[0u][0u] + n[1u][0u];
	const size_t n2 = n[0u][1u] + n[1u][1u];

	// Early exit
	if (!n1 || !n2) return 0.0;

	// Useful sums
	const double sumx0 = sumx[0u][0u] + sumx[0u][1u] + sumx[1u][0u] + sumx[1u][1u]; 
	const double sumx1 = sumx[0u][0u] + sumx[1u][0u];
	const double sumx2 = sumx[0u][1u] + sumx[1u][1u];

	// Useful sums of squares	
	const double ssqx0 = ssqx[0u][0u] + ssqx[0u][1u] + ssqx[1u][0u] + ssqx[1u][1u];
	const double ssqx1 = ssqx[0u][0u] + ssqx[1u][0u];
	const double ssqx2 = ssqx[0u][1u] + ssqx[1u][1u];

	// Variances
	double varx0 = ssqx0 / n0 - utl::sqr(sumx0 / n0);
	double varx1 = ssqx1 / n1 - utl::sqr(sumx1 / n1);
	double varx2 = ssqx2 / n2 - utl::sqr(sumx2 / n2);

	// Correct small numerical imprecisions
	varx0 = utl::correct(varx0);
	varx1 = utl::correct(varx1);
	varx2 = utl::correct(varx2);

	// Make sure the variances are positive
	assert(varx0 >= 0.0);
	assert(varx1 >= 0.0);
	assert(varx2 >= 0.0);

	// Compute the F-statistic
	double EI = 1.0 - (n1 * varx1 + n2 * varx2) / (n0 * varx0);

	// Check
	assert(EI >= 0.0 && EI <= 1.0);

	// Lower bound
	const double EI0 = 2.0 / utl::pi(); 

	// Rescale it
	EI = (EI - EI0) / (1.0 - EI0);

	// Re-check
	assert(EI <= 1.0);

	return EI;

}

// Function to compute the spatial isolation statistic
double stat::si(const std::vector<std::vector<size_t> > &n) {

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
	const double a = n11 * n22;
	const double b = n12 * n21;
	const double SI = prod ? fabs((a - b) / sqrt(prod)) : 0.0;

	// Note: somehow huge numerical imprecision errors if I don't compute a and b separately

	// Make sure spatial isolation is between zero and one
	assert(SI >= 0.0 && SI <= 1.0);

	return SI;

}

// Function to perform one step of the life cycle
void Population::cycle(Printer &print) {

    // print: a printer

	// Note:
	// There are multiple feeding rounds.
	// Every feeding round, individuals are taken in random order.
	// Each individual assesses its expected fitness if feeding on each resource.
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
	// expected one but now with sums of feeding efficiencies being computed with everybody's values
	// throughout all feeding rounds.

    // Flag to know if it is time to save some data
    const bool tts = print.ison() && time % tsave == 0u;

    // Save time step if needed
    if (tts) print.save("time", static_cast<double>(time));

    // Check
    assert(individuals->size() == popsize);

    // Initialize a vector of fitnesses
	std::vector<double> fitnesses(popsize);

    // Initialize a vector of consecutive indices
	std::vector<size_t> indices(popsize);
	std::iota(indices.begin(), indices.end(), 0u);

 	// For each feeding round...
	for (size_t j = 0u; j < nrounds; ++j) {

		// Shuffle indices to take individuals in random order
		std::shuffle(indices.begin(), indices.end(), rnd::rng);

		// Initialize cumulative feeding efficiencies in each habitat on each resource
		std::vector<std::vector<double> > sumeffs({{0.0, 0.0}, {0.0, 0.0}});

		// Initialize other useful containers
		std::vector<std::vector<size_t> > n({{0u, 0u}, {0u, 0u}});
		std::vector<std::vector<double> > sumx({{0.0, 0.0}, {0.0, 0.0}}); 

		// For each individual...
		for (size_t i = 0u; i < popsize; ++i) {        

			// Respect random order
			const size_t ii = indices[i];

            // Isolate focal individual
            Individual& ind = (*individuals)[ii];

			// Assign a rank in the queue to the individual
			ind.setRank(i);

			// Read individual properties
			const double x = ind.getX();
			const bool habitat = ind.getHabitat();

			// Get feeding efficiency on each resource
			const std::vector<double> effs({ ind.getEff1(), ind.getEff2() });

			// Compute the cumulative consumption rates so far on each resource (incl. focal individual)		
			const double cumul1 = sumeffs[habitat][0u] + effs[0u];
			const double cumul2 = sumeffs[habitat][1u] + effs[1u];

			// Check
			assert(cumul1 >= effs[0u]);
			assert(cumul2 >= effs[1u]);

			// Compute the amount of resource discovered for each resource
			const double discov1 = pop::discover(resources[habitat][0u], delta, cumul1);
			const double discov2 = pop::discover(resources[habitat][1u], delta, cumul2);

			// Check
			assert(discov1 >= 0.0);
			assert(discov2 >= 0.0);

			// Check that the resource discovery function is indeed saturating
			assert(discov1 <= resources[habitat][0u]);
			assert(discov2 <= resources[habitat][1u]);

			// Compute expected fitness on each resource (special case when denominator is zero)
			const double fit1 = pop::fitness(discov1, effs[0u], cumul1, n[habitat][0u] + 1u);
			const double fit2 = pop::fitness(discov2, effs[1u], cumul2, n[habitat][1u] + 1u);

			// Check that expected fitnesses are above zero
			assert(fit1 >= 0.0);
			assert(fit2 >= 0.0);

			// Record expected fitness difference
			ind.setDiff(fit2 - fit1);

			// Make the individual choose
			ind.makeChoice(fit1, fit2, alpha, beta, resources[habitat][0u], resources[habitat][1u]);

			// Read the choice that was made
			const bool choice = ind.getChoice();

			// Update cumulative feeding efficiencies depending on what resource has been chosen, in that habitat
			sumeffs[habitat][choice] += effs[choice];

			// Update other important statistics
			++n[habitat][choice];
			sumx[habitat][choice] += x;

		}

		// Save the number and mean trait values of individuals feeding on each resource in each habitat if needed
		if (tts) {
			for (size_t i = 0u; i < 2u; ++i) {
				for (size_t k = 0u; k < 2u; ++k) {
					print.save("resourceCensus", static_cast<double>(n[i][k]));
					print.save("resourceMeanTraitValue", n[i][k] ? sumx[i][k] / n[i][k] : 0.0);
				}
			}
		}

		// Compute the final amounts of resources discovered in each habitat on each resource
		std::vector<std::vector<double> > discovered({{0.0, 0.0}, {0.0, 0.0}});
		for (size_t i = 0u; i < 2u; ++i) {
			for (size_t k = 0u; k < 2u; ++k) {
				discovered[i][k] = pop::discover(resources[i][k], delta, sumeffs[i][k]);
				assert(discovered[i][k] >= 0.0);
				assert(discovered[i][k] <= resources[i][k]);
			}
		}

		// For each individual...
		for (size_t i = 0u; i < popsize; ++i) {

            // Isolate focal individual
            Individual& ind = (*individuals)[i];

			// Read relevant individual properties
			const bool choice = ind.getChoice();
			const bool habitat = ind.getHabitat();
			const bool diff = ind.getDiff();
			const size_t rank = ind.getRank();

			// Corresponding feeding efficiency
			const double eff = choice ? ind.getEff2() : ind.getEff1();

			// Compute realized fitness on the chosen resource
			const double fit = pop::fitness(discovered[habitat][choice], eff, sumeffs[habitat][choice], n[habitat][choice]);

			// Check that the fitness is above zero
			assert(fit >= 0.0);

			// Add obtained food to the vector of fitnesses
			fitnesses[i] += fit;

			// Save a few individual properties if needed
			if (tts) {
						
				print.save("individualExpectedFitnessDifference", diff);
				print.save("individualChoice", static_cast<double>(choice));
				print.save("individualRealizedFitness", fit);
				print.save("individualRank", static_cast<double>(rank));

            }

			// Set individual ecotype relative to population average while we are looping through individuals
			if (!j) ind.setEcotype((sumx[0u][0u] + sumx[0u][1u] + sumx[1u][0u] + sumx[1u][1u]) / popsize);

		}
	}

    // Create a distribution to sample parents from proportionately to fitness
	auto sampleParent = rnd::discrete(fitnesses.begin(), fitnesses.end());

	// Initialize useful containers for habitat- and ecotype-specific statistics
	std::vector<std::vector<size_t> > n({{0u, 0u}, {0u, 0u}});
	std::vector<std::vector<double> > sumx({{0.0, 0.0}, {0.0, 0.0}});
	std::vector<std::vector<double> > ssqx({{0.0, 0.0}, {0.0, 0.0}});

    // Prepare space to welcome newborns
    newborns->reserve(popsize);

	// For each individual to be born...
	for (size_t i = 0u; i < popsize; ++i) {

		// Sample parent of the current offspring (with replacement)
		const size_t j = sampleParent(rnd::rng);

        // Isolate the parent
        Individual& parent = (*individuals)[j];

		// Add offspring to the population by cloning
		newborns->push_back(parent);

		// Mutate offspring if needed
		if (rnd::bernoulli(mutrate)(rnd::rng))
			newborns->back().mutate(sampleMutation(rnd::rng), tradeoff);

		// The offspring has a chance to disperse
		if (rnd::bernoulli(dispersal)(rnd::rng))
			newborns->back().disperse();

        // Isolate the adult with the same index (for data collection)
        Individual& ind = (*individuals)[i];

		// Get relevant individual metrics for the corresponding adult
		const double x = ind.getX();
		const bool habitat = ind.getHabitat();
		const bool ecotype = ind.getEcotype();
     
		// Update habitat- and ecotype-specific statistics
		++n[habitat][ecotype];
		sumx[habitat][ecotype] += x;
		ssqx[habitat][ecotype] += utl::sqr(x);

		// Save a few individual properties of that adult if needed
		if (tts) {
					
			print.save("individualHabitat", static_cast<double>(habitat));
			print.save("individualTraitValue", x);
			print.save("individualTotalFitness", fitnesses[i]);
			print.save("individualEcotype", static_cast<double>(ecotype));

		}
    }

	// Compute statistics
	const double EI = stat::ei(n, sumx, ssqx);
	const double SI = stat::si(n);

	// Save some population-level statistics if needed
	if (tts) {

		// Save the number and mean trait values of individuals in each habitat
		for (size_t i = 0u; i < 2u; ++i) {

			const size_t n0 = n[i][0u] + n[i][1u];
			print.save("habitatCensus", static_cast<double>(n0));
			print.save("habitatMeanTraitValue", n0 ? (sumx[i][0u] + sumx[i][1u]) / n0 : 0.0);

		}

		// Save ecological isolation
		print.save("ecologicalIsolation", EI);

		// Save spatial isolation
		print.save("spatialIsolation", SI);

	}

	// Verbose if needed
    if (verbose) {

		// Show statistics
    	std::cout << "t = " << time << ", ";
		std::cout << "meanx = " << (sumx[0u][0u] + sumx[0u][1u] + sumx[1u][0u] + sumx[1u][1u]) / popsize << ", ";
		std::cout << "EI = " << EI << ", ";
		std::cout << "SI = " << SI << '\n';

	}

    // Newborns become adults
    std::swap(individuals, newborns);

    // No more newborns (adults die) 
    newborns->clear();

    // Check
    assert(newborns->empty());

	// Make sure population size has not changed
	assert(individuals->size() == popsize);
    
}