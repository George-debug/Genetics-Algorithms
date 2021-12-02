#pragma once

#include "runInfo.h"

class Crossover {
protected:
	size_t _input, _output;

public:
	Crossover(size_t input, size_t output);
	size_t getInput();
	size_t getOutput();
	virtual runInfo::Generation operator()(const runInfo::Generation& individuals) = 0;
};


runInfo::Generation useCrossover(const runInfo::Generation& gen, size_t wantedSize, std::shared_ptr<Crossover> cross);