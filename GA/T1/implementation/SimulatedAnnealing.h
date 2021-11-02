#pragma once
#include <vector>
#include <memory>
#include <cmath>

#include "GlobalOptima.h"
#include "MultidimensionalVector.h"

#define phi2 0.61803

template<typename DomainType, typename RangeType>
class SimulatedAnnealing : public GlobalOptimaMV<DomainType, RangeType>
{
	using timePointDuration = typename GlobalOptima<DomainType, RangeType>::timePointDuration;
	using timePoint = typename GlobalOptima<DomainType, RangeType>::timePoint;
	using useDataFunctionType = typename GlobalOptima<DomainType, RangeType>::useDataFunctionType;
	using quantifyFunctionType = typename GlobalOptima<DomainType, RangeType>::quantifyFunctionType;
	using MV = typename GlobalOptimaMV<DomainType, RangeType>::MV;
	using MVPointer = typename GlobalOptimaMV<DomainType, RangeType>::MVPointer;

protected:
	const double c_tempStart, c_alpha;
	static std::uniform_real_distribution<double> s_random01;

	double getProbability(double delta, double temp) {
		if (delta < 0)
			return 1;
		
		return exp(-(delta / temp));
	}

public:
	SimulatedAnnealing(MV* mv, double temperature, double alpha)
		: GlobalOptimaMV<DomainType, RangeType>(mv), c_tempStart(temperature), c_alpha(alpha) {}

	void run(quantifyFunctionType func, useDataFunctionType useData) override {
		this->resetTimer();
		
		MVPointer bestMV = this->_vector_instance->generateRandom();
		std::vector<DomainType> bestDomain = bestMV->operator std::vector<DomainType>();
		RangeType bestVal = func(bestDomain), globalBest = bestVal;

		useData(this->getCurrentTimer(), bestVal);

		RangeType t = c_tempStart;

		do{
			auto successors = bestMV->successors();
			shuffle(successors.begin(), successors.end(), GlobalOptimaMV<DomainType, RangeType>::s_randomEngine);

			for (MVPointer neighbor : successors) {
				std::vector<DomainType> neighborDomain = neighbor->operator std::vector<DomainType>();
				RangeType neighborVal = func(neighborDomain), diff = neighborVal - bestVal;
				if (abs(diff) < 0.00001) continue;
				double p = getProbability(diff, t);
				
				//std::cout << this->getCurrentTimer() << " | T = " << t << " | D = " << neighborVal - bestVal << " | " << "P = " << p << std::endl;

				
				//std::cout << "T = " << t << std::endl;
				if (s_random01(GlobalOptimaMV<DomainType, RangeType>::s_randomEngine) <= p) {
					bestVal = neighborVal;
					bestMV = neighbor;

					if (bestVal < globalBest) {
						globalBest = bestVal;
						useData(this->getCurrentTimer(), bestVal);
					}
					break;
					
				}
				//t = t * c_alpha;
			}
			t = t / (1 + t * c_alpha);
			//t = t * c_alpha;
			
			
		} while (t > 0.03);
	}
};

template<typename DomainType, typename RangeType>
std::uniform_real_distribution<double> SimulatedAnnealing<DomainType, RangeType>::s_random01 = std::uniform_real_distribution<double>(0, 1);