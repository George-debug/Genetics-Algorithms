#pragma once
#include <vector>
#include <queue>
#include <iostream>
#include <iomanip> //fixed and setprecition


//When u have time use a class instead of this mess and use pointers
//In case u'll come back to this
//u can see if using boolean for direction is somehow faster
//rethink the variable's name

bool notEqual(long double a, long double b);
bool minComp(long double a, long double b);
bool maxComp(long double a, long double b);

/*
	100, 50, 10, 1, 0.5, 0.1
	0.1, 0.5, 1, 10, 50, 100
*/

template<typename domainType>
struct AxisVector {
	domainType direction;
	size_t index;
	std::vector<domainType> value;

	AxisVector(size_t dimensions, size_t _index, domainType _direction) :
		direction(_direction), index(_index), value(dimensions)
	{}

	AxisVector(const std::vector<domainType>& _value, size_t _index, domainType _direction) :
		direction(_direction), index(_index), value(_value)
	{}
};

template<typename domainType>
std::vector<AxisVector<domainType>> generateSuccessors(const AxisVector<domainType>& av, const std::vector<domainType>& steps) {
	std::vector<AxisVector<domainType>> q;


	//front
	for (const domainType& step : steps)
	{
		AxisVector<domainType> next = av;
		next.value[next.index] += step * next.direction;
		q.push_back(next);
	}

	if (av.index == av.value.size() - 1)
		return q;

	//"left" and "right"

	for (const domainType& step : steps)
	{
		AxisVector<domainType> next = av;
		next.value[next.index + 1] += step;
		next.direction = 1;
		++next.index;
		q.push_back(next);

		next = av;
		next.value[next.index + 1] -= step;
		next.direction = -1;
		++next.index;
		q.push_back(next);

	}
	return q;
}

template<typename domainType>
std::queue<AxisVector<domainType>> positionToAxisVectorQueue(const std::vector<domainType>& startingPosition) {
	std::queue<AxisVector<domainType>> q;
	AxisVector<domainType> startingVector(startingPosition, 0, 1);

	q.push(startingVector);

	startingVector.direction = -1;
	q.push(startingVector);

	return q;
}


template<typename domainType>
void hillClimbingEnforced(size_t numberOfVars, long double(*funcPtr)(const std::vector<domainType>&), bool getMax = true, std::vector<domainType> startingPosition = {}, std::vector<domainType> steps = { 13, 0.8, 0.1 }) {
	bool(*compFunc)(long double, long double) = getMax ? maxComp : minComp;

	if (startingPosition.size() != numberOfVars)
	{
		if (startingPosition.size() != 0)
			std::cout << "Warning: Your Input doesn't have the exact number of variables" << std::endl;
		startingPosition = std::vector<domainType>(numberOfVars);
	}

	std::queue<AxisVector<domainType>> openQueue = positionToAxisVectorQueue(startingPosition);
	//std::vector<domainType> bestVector = startingPosition;
	long double bestVectorValue = funcPtr(startingPosition);

	for (auto i : openQueue.front().value)
		std::cout << i << " " << std::fixed << std::setprecision(3);
	std::cout << ">> " << bestVectorValue << std::endl;

	while (!openQueue.empty()) {
		AxisVector<domainType> currentVector = openQueue.front();
		openQueue.pop();
		std::vector<AxisVector<domainType>> successors;
		successors = generateSuccessors(currentVector, steps);

		for (const AxisVector<domainType>& nextVector : successors) {
			auto nextVectorValue = funcPtr(nextVector.value);

			if(compFunc(nextVectorValue, bestVectorValue) && notEqual(nextVectorValue, bestVectorValue)) {
				bestVectorValue = nextVectorValue;
				openQueue = positionToAxisVectorQueue(nextVector.value);

				for (auto i : nextVector.value)
					std::cout << i << " " << std::fixed << std::setprecision(3);
				std::cout << ">> " << bestVectorValue << std::endl;

				break;
			}
			//for (auto i : nextVector.value)
			//	std::cout << i << " ";
			//std::cout <<  " | i = " << nextVector.index << " | d = " << nextVector.direction << " | " << nextVectorValue << std::endl;
			openQueue.push(nextVector);
		}
	}
}