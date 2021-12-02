#pragma once

#include "Selector.h"
#include "details.h"
#include "runInfo.h"

template<size_t _bits>
class RWSelector : public Selector {
protected:
	const double c_eliteRatio;

public:
	RWSelector(double eliteRatio) : c_eliteRatio(std::max(std::min(eliteRatio, 1.0), 0.0)) {}

	//we asume that Generation is sorted
	runInfo::Generation operator()(runInfo::Generation oldGeneration, size_t wantedSize, size_t& eliteSize) override {
		runInfo::Generation newGeneration;
		newGeneration.reserve(wantedSize);

		eliteSize = static_cast<size_t>(ceil(oldGeneration.size() * c_eliteRatio));

		for (size_t i = 0; i < eliteSize; ++i)
			newGeneration.push_back(oldGeneration[i]);

		//########################
		std::vector<double> form;
		form.reserve(oldGeneration.size() - eliteSize);
		double formSum = 0;
		
		//for (size_t i = eliteSize; i < oldGeneration.size(); ++i)
		//	from.push();
		//########################

		return newGeneration;
	}
};