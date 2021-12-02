#pragma once

#include "details.h"
#include "runInfo.h"

class Selector {
public:

	//we asume that Generation is sorted
	virtual runInfo::Generation operator()(const runInfo::Generation& oldGeneration, size_t wantedSize);
};