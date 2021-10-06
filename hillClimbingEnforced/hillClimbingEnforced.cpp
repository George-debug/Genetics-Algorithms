#include "hillClimbingEnforced.h"

#define myEpsilon 2.2e-10

bool notEqual(long double a, long double b) {
	return abs(a - b) > myEpsilon;
}

bool minComp(long double a, long double b) {
	return a < b;
}


bool maxComp(long double a, long double b) {
	return a > b;
}

