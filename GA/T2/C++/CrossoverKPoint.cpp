#include "CrossoverKPoint.h"
#include "runInfo.h"

CrossoverKPoint::CrossoverKPoint(const std::vector<size_t>& points) : _points(points), Crossover(2, 2)
{}

runInfo::Generation CrossoverKPoint::operator()(const runInfo::Generation& individuals) {
	size_t
		dimensions = individuals[0].first->size(),
		currentStepIndex = 0,
		currentBit = 0;
	auto kid1 = std::make_shared<runInfo::Individual>(dimensions);
	auto kid2 = std::make_shared<runInfo::Individual>(dimensions);
	//double auxIndivValue = converter(*auxIndiv);
	for(size_t d = 0 ; d < dimensions ; ++d)
		for (size_t b = 0; b < runInfo::bitSize; ++b) {
			kid1->at(d)._representation[b] = individuals[0].first->at(d)._representation[b];
			kid2->at(d)._representation[b] = individuals[1].first->at(d)._representation[b];

			if (currentBit == _points[currentStepIndex]) {
				std::swap(kid1, kid2);
				
				if (currentStepIndex != _points.size() - 1)
					++currentStepIndex;
			}
			++currentBit;
		}

	return { std::make_pair(kid1, -1), std::make_pair(kid2, -1) };
}