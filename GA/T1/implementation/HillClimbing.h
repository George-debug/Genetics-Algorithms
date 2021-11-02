#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "GlobalOptima.h"
#include "MultidimensionalVector.h"

template <typename DomainType, typename RangeType>
class HillClimbing : public GlobalOptimaMV<DomainType, RangeType> {
	using timePointDuration = typename GlobalOptima<DomainType, RangeType>::timePointDuration;
	using timePoint = typename GlobalOptima<DomainType, RangeType>::timePoint;
	using useDataFunctionType = typename GlobalOptima<DomainType, RangeType>::useDataFunctionType;
	using quantifyFunctionType = typename GlobalOptima<DomainType, RangeType>::quantifyFunctionType;
	using MV = typename GlobalOptimaMV<DomainType, RangeType>::MV;
	using MVPointer = typename GlobalOptimaMV<DomainType, RangeType>::MVPointer;

protected:
	using improvementFunctionType = MVPointer(*)(std::vector<MVPointer>&, RangeType&, quantifyFunctionType);
	
	improvementFunctionType _improvement;
	const size_t c_iterations;

public:
	HillClimbing(MV* mv, size_t iterations, improvementFunctionType improvement)
		:  GlobalOptimaMV<DomainType, RangeType>(mv), c_iterations(iterations) {
		_improvement = improvement;
	};

	static MVPointer firstImprovement(std::vector<MVPointer>& candidates, RangeType& best, quantifyFunctionType func ) {
		std::shuffle(candidates.begin(), candidates.end(), GlobalOptimaMV<DomainType, RangeType>::s_randomEngine);

		for (auto c : candidates) {
			RangeType cVal = func(c->operator std::vector<DomainType>());
			if (cVal < best && abs(cVal-best) >= 0.00001) {
				best = cVal;
				return c;
			}
		}

		return nullptr;
	}

	static MVPointer bestImprovement(std::vector<MVPointer>& candidates, RangeType& best, quantifyFunctionType func) {
		MVPointer rv = nullptr;

		for (auto c : candidates) {
			RangeType cVal = func(c->operator std::vector<DomainType>());

			if (cVal < best) {
				best = cVal;
				rv = c;
			}
		}

		return rv;
	}


	void run(quantifyFunctionType func, useDataFunctionType useData) override {
		//std::cout << "HillClimbing:" << std::endl;
		this->resetTimer();

		MVPointer bestMV = this->_vector_instance->generateRandom();
		auto bestDomain = bestMV->operator std::vector<DomainType>();
		RangeType bestVal = func(bestDomain);

		useData(this->getCurrentTimer(), bestVal);
		size_t it = 0;
		while (it < c_iterations) {
			bool local = false;
			MVPointer candidate = this->_vector_instance->generateRandom();
			auto candidateDomain = candidate->operator std::vector<DomainType>();
			RangeType candidateVal = func(candidateDomain);

			do {
				auto successors = candidate->successors();
				MVPointer bestSuccessor = _improvement(successors, candidateVal, func);

				if (bestSuccessor != nullptr)
					candidate = bestSuccessor;
				else
					local = true;
				
			} while (local == false);

			if (candidateVal < bestVal) {
				bestVal = candidateVal;
				useData(this->getCurrentTimer(), bestVal);
			}
			++it;
		}
	}
};