#pragma once
#include <vector>
#include <memory>
#include <iostream> //!!

template <typename DomainType>
class MultidimensionalVector {
public:
	virtual std::shared_ptr<MultidimensionalVector<DomainType>> generateRandom() {
		return NULL;
	}
	virtual std::vector<std::shared_ptr<MultidimensionalVector<DomainType>>> successors() {
		std::vector<std::shared_ptr<MultidimensionalVector<DomainType>>> x;
		return x;
	}
	virtual operator std::vector<DomainType>() {
		std::vector<DomainType> x;
		return x;
	}
};
