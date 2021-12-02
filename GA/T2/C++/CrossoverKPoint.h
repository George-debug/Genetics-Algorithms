#pragma once

#include <vector>

#include "Crossover.h"

class CrossoverKPoint : public Crossover {
protected:
	//size_t _dimensions;
	std::vector<size_t> _points;

public:
	CrossoverKPoint(const std::vector<size_t>& points);
	runInfo::Generation operator()(const runInfo::Generation& individuals) override;
};