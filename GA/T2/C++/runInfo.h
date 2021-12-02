#pragma once

#include "BinaryChromosome.h"

namespace runInfo {
	static const size_t
		bitSize = 28,
		mutationChance = 10000,
		iterations = 1000,
		popSize = 300,
		numberOfTests = 30;

	static double myEpsilon = 0.00001;

	using Chromosome = BinaryChromosome<bitSize, mutationChance>;

	using Individual =  std::vector<Chromosome>;

	using Solution = std::pair<std::shared_ptr<Individual>, double>;

	using Generation = std::vector<Solution>;

	//using FunctionUseSolution = std::function<void(const Generation&)>;

	using FunctionFitness = std::function<double(const std::vector<double>& x)>;

	using FunctionConverter = std::function<double(const Individual& x)>;

	bool insertSorted(Generation& v, const Solution& n);
	void printGeneration(const runInfo::Generation& g);
	void sortGeneration(Generation& gen);
}