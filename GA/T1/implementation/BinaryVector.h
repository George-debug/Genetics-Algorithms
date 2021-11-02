#pragma once
#include <vector>
#include <iostream> // !!
#include <random>
#include <memory>
#include <algorithm>

#include "MultidimensionalVector.h"
#include "details.h"

template<typename UIntType>
using UIntGeneratorType = std::independent_bits_engine<std::mt19937, 8 * sizeof(UIntType), UIntType>;

template<typename UIntType, typename DomainType>
class BinaryVector : public MultidimensionalVector<DomainType> {
protected:
	std::vector<UIntType> _value;
	//static const UIntType c_maxValue;
	static const size_t s_bitSize;
	static const std::vector<UIntType> s_eachBitOn;
	const size_t c_dimensions;
	static UIntGeneratorType<UIntType> s_UIntTypeRandomGenerator;
	std::shared_ptr<details::UIntConverter<UIntType, DomainType>> c_converter;

public:
	BinaryVector(size_t dimensions, DomainType lowerLimit, DomainType upperLimit) : c_dimensions(dimensions) {
		_value = generateRandomValue();
		UIntType UIntMaxVal = ~UIntType(0);
		DomainType change = upperLimit - lowerLimit, maxVal = DomainType(UIntMaxVal);

		//std::cout << "change = " << change << std::endl;
		//std::cout << "maxVal = " << maxVal << std::endl;
		//std::cout << "real max val = " << UIntMaxVal << std::endl;
		//std::cout << "lowerLimit = " << lowerLimit << std::endl;

		c_converter = std::make_shared<details::UIntConverter<UIntType, DomainType>>([maxVal, change, lowerLimit](UIntType x) {
			return (x / maxVal) * change + lowerLimit;
			});
	}

	BinaryVector(size_t dimensions, const std::shared_ptr<details::UIntConverter<UIntType, DomainType>>& coverter) : c_dimensions(dimensions), c_converter(coverter) {
		_value = generateRandomValue();
	}

	BinaryVector(const BinaryVector<UIntType, DomainType>& right)
		: c_dimensions(right.c_dimensions), _value(right._value), c_converter(right.c_converter) {
		//std::cout << "Copied!" << std::endl;
	}

	std::vector<UIntType> generateRandomValue() const {
		std::vector<UIntType> rv(c_dimensions);

		for (size_t i = 0; i < c_dimensions; ++i)
			rv[i] = s_UIntTypeRandomGenerator();

		return rv;
	}

	static void switchBit(UIntType& val, size_t bitIndex) { // const UIntType & ??
		val ^= s_eachBitOn[bitIndex];
	}

	std::shared_ptr<MultidimensionalVector<DomainType>> generateRandom() override {
		std::shared_ptr<MultidimensionalVector<DomainType>> rv = std::make_shared<BinaryVector<UIntType, DomainType>>(c_dimensions, c_converter);
		
		//rv._value = generateRandomValue();
		
		return rv;
	}

	std::vector<std::shared_ptr<MultidimensionalVector<DomainType>>> successors() override {
		std::vector<std::shared_ptr<MultidimensionalVector<DomainType>>> rv;
		rv.reserve(s_bitSize * c_dimensions);

		for (size_t d = 0; d < c_dimensions; ++d)
			for (size_t b = 0; b < s_bitSize; ++b) {
				std::shared_ptr<BinaryVector<UIntType, DomainType>> aux = std::make_shared<BinaryVector<UIntType, DomainType>>(*this);
				switchBit (aux->_value[d], b);

				rv.push_back(std::move(aux));
			}

		//for (std::shared_ptr<MultidimensionalVector<double>> i : rv)
		//	i->print();

		return rv;
	}

	operator std::vector<DomainType>() override {
		std::vector<DomainType> rv(c_dimensions);
		std::transform(_value.begin(), _value.end(), rv.begin(), *c_converter);
		return rv;
	};
};

template<typename UIntType, typename DomainType>
const std::vector<UIntType> BinaryVector<UIntType, DomainType>::s_eachBitOn = details::generateEachBit<UIntType>();

template<typename UIntType, typename DomainType>
const size_t BinaryVector<UIntType, DomainType>::s_bitSize =8 * sizeof(UIntType);

//template<typename UIntType, typename DomainType>
//const UIntType BinaryVector<UIntType, DomainType>::c_maxValue = ~UIntType(0);

template<typename UIntType, typename DomainType>
UIntGeneratorType<UIntType> BinaryVector<UIntType, DomainType>::s_UIntTypeRandomGenerator = UIntGeneratorType<UIntType>(details::s_randomDevice());