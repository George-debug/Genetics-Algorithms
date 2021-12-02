#pragma once

#include <functional>
#include <vector>
#include <memory>

#include "BinaryChromosome.h"
#include "runInfo.h"
#include "Selector.h"
#include "Crossover.h"

size_t findSelectSize(size_t populationSize, size_t inputSize, size_t outputSize);

template<typename CoverterType>
class GeneticAlgorithm {
private:
	const size_t c_populationSize, c_dimensions, c_selectSize;
	std::shared_ptr<Selector> _select;
	std::shared_ptr<Crossover> _crossover;
	std::shared_ptr<runInfo::Chromosome> _chromosome;
	CoverterType _convert;
	runInfo::Generation _gen;

	void resetGeneration() {
		_gen.clear();
		_gen.reserve(c_populationSize);

		while (_gen.size() != c_populationSize) {
			auto auxIndiv = std::make_shared<runInfo::Individual>(c_dimensions);
			double auxIndivValue = _convert(*auxIndiv);
			runInfo::Solution auxSol = std::make_pair(auxIndiv, auxIndivValue);

			runInfo::insertSorted(_gen, auxSol);
		}
	}

public:
	GeneticAlgorithm(
		size_t populationSize,
		size_t dimensions,
		std::shared_ptr<Selector> select,
		std::shared_ptr<Crossover> crossover,
		std::shared_ptr<runInfo::Chromosome> chromosome,
		const CoverterType& convert
	) :
		c_populationSize(populationSize),
		c_dimensions(dimensions),
		_select(select),
		_crossover(crossover),
		_chromosome(chromosome),
		_convert(convert),
		c_selectSize(findSelectSize(c_populationSize, crossover->getInput(), crossover->getOutput()))
	{
		resetGeneration();
	}

	double getBestOfGeneration() {
		return _gen[0].second;
	}

	void nextGeneration() {
		_gen = _select->operator()(_gen, c_selectSize);
		_gen = useCrossover(_gen, c_populationSize, _crossover);

		for (auto& indi : _gen) {
			for (runInfo::Chromosome& ch : *(indi.first))
				ch.mutate();

			double aux = _convert(*(indi.first));
			indi.second = aux;
		}
		runInfo::sortGeneration(_gen);
	}

};



