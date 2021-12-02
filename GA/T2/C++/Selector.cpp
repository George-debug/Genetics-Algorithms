#include <algorithm>

#include "Selector.h"

runInfo::Generation Selector::operator()(const runInfo::Generation& oldGeneration, size_t wantedSize) {
	runInfo::Generation newGeneration;
	newGeneration.reserve(wantedSize);

	for (size_t i = 0; i < wantedSize; ++i)
		newGeneration.push_back(oldGeneration[i]);

	return newGeneration;
}