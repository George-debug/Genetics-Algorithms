
#include "Crossover.h"

Crossover::Crossover(size_t input, size_t output) : _input(input), _output(output) {}

size_t Crossover::getInput() {
	return _input;
}
size_t Crossover::getOutput() {
	return _output;
}

void printParents(const std::vector<size_t>& v) {
	for (size_t i : v)
		std::cout << i << " ";
	std::cout << std::endl;
}

void nextParents(std::vector<size_t>& index, const std::vector<size_t>& stopIndex, size_t i) {
	++index[i];
	if (index[i] == stopIndex[i]) {
		if (i == 0) return;
		nextParents(index, stopIndex, i-1);
		index[i] = index[i - 1] + 1;
	}
}

void addIndividuals(runInfo::Generation& gen, const runInfo::Generation& addThis) {
	for (const runInfo::Solution& sol : addThis) {
		gen.push_back(sol);
	}
}

runInfo::Generation useCrossover(const runInfo::Generation& gen, size_t wantedSize, std::shared_ptr<Crossover> cross) {
	runInfo::Generation rv;
	rv.reserve(wantedSize);

	size_t input = cross->getInput(), genSize = gen.size();
	runInfo::Generation parents;
	parents.reserve(input);

	std::vector<size_t> index, stopIndex;
	index.reserve(input);
	stopIndex.reserve(input);

	for (size_t i = 0; i < input; ++i) {
		index.push_back(i);
		stopIndex.push_back(genSize + i - input + 1);
		parents.push_back(gen[i]);
	}

	addIndividuals(rv, cross->operator()(parents));
	
	//printParents(index);
	//printParents(stopIndex);

	while (rv.size() < wantedSize) {

		nextParents(index, stopIndex, input - 1);

		if (index[0] == stopIndex[0])
			break;

		//printParents(index);

		for (size_t i = 0; i < input; ++i)
			parents[i] = gen[index[i]];

		addIndividuals(rv, cross->operator()(parents));
	}

	return rv;
}