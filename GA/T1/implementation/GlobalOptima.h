#pragma once

#include <memory>
#include <chrono>
#include <random>

#include "details.h"
#include "MultidimensionalVector.h"

template <typename DomainType, typename RangeType>
class GlobalOptima {
protected:
	using timePointDuration = std::chrono::duration<double>;
	using timePoint = std::chrono::time_point<std::chrono::steady_clock>;
	using useDataFunctionType = void(*)(double, RangeType);
	using quantifyFunctionType = RangeType(*)(const std::vector<DomainType>&);

	//const quantifyFunctionType c_quantify;
	//const useDataFunctionType c_useData;
	timePoint _startTime;

public:
	
	virtual void run(quantifyFunctionType func, useDataFunctionType useData) {
		resetTimer();

		for (size_t i = 0; i < 100; ++i) {
			for (size_t j = 0; j < 100000000; ++j) {}
			//c_useData(getCurrentTimer(), 0, 0);
		}
	};

	void resetTimer() {
		_startTime = std::chrono::steady_clock::now();
	}

	double getCurrentTimer() const {
		timePoint _done_time = std::chrono::steady_clock::now();
		return timePointDuration(_done_time - _startTime).count();
	}
	
	//void newOptimalValue(const std::vector<DomainType>& d, const RangeType& r ) const {
	//	c_useData(getCurrentTimer(), d, r);
	//}

};

template <typename DomainType, typename RangeType>
class GlobalOptimaMV : public GlobalOptima<DomainType, RangeType> {
	using timePointDuration = typename GlobalOptima<DomainType, RangeType>::timePointDuration;
	using timePoint = typename GlobalOptima<DomainType, RangeType>::timePoint;
	using useDataFunctionType = typename GlobalOptima<DomainType, RangeType>::useDataFunctionType;
	using quantifyFunctionType = typename GlobalOptima<DomainType, RangeType>::quantifyFunctionType;

protected:
	using MV = MultidimensionalVector<DomainType>;
	using MVPointer = std::shared_ptr<MV>;

	MV * _vector_instance;
	static std::mt19937 s_randomEngine;

public:
	GlobalOptimaMV(MV* mv)
		: _vector_instance(mv) {
	}
};

template <typename DomainType, typename RangeType>
std::mt19937 GlobalOptimaMV<DomainType, RangeType>::s_randomEngine = std::default_random_engine(details::s_randomDevice());