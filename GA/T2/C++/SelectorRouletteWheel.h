#pragma once
#include "Selector.h"
#include "runInfo.h"

class SelectorRouletteWheel : public Selector {
public:

	//we asume that Generation is sorted
	runInfo::Generation operator()(const runInfo::Generation& oldGeneration, size_t wantedSize);
};