#pragma once

//pur si sumplu nu mai pot, fac cu struct

#include <iostream> //!!
#include <bitset> 
#include <vector>
#include <cmath>

#include "details.h"

#define UIntType unsigned long



template<size_t _bits, size_t _chanceOfMutationInOneMilion>
class BinaryChromosome {
protected:
	static const double s_mutationProbability;

	void setRandomRepresentation() {
		_representation = std::bitset<_bits>(static_cast<UIntType>(std::numeric_limits<UIntType>::max() * details::getRandom01(details::s_randomDevice)));
	}

public:
	//dupa 6 zile nu mai am rabdare, ramane public
	std::bitset<_bits> _representation;
	//bool itChanged;

	BinaryChromosome() {
		setRandomRepresentation();
		//itChanged = true;
	}

	BinaryChromosome(UIntType val) {
		_representation = std::bitset<_bits>(val);
		//itChanged = true;
	}

	std::shared_ptr<BinaryChromosome<_bits, _chanceOfMutationInOneMilion>> generateNewIndividual()  {
		return std::make_shared< BinaryChromosome<_bits, _chanceOfMutationInOneMilion>>();
	};

	void mutate()  {
		for (size_t i = 0; i < _bits; ++i)
			if (details::getRandom01(details::s_randomDevice) <= s_mutationProbability) {
				_representation.flip(i);
				//itChanged = true;
			}
	};

	operator unsigned long long() const {
		return _representation.to_ullong();
	}

	operator unsigned long() const {
		return _representation.to_ulong();
	}
};

template<size_t _bits, size_t _chanceOfMutationInOneMilion>
const double BinaryChromosome<_bits, _chanceOfMutationInOneMilion>::s_mutationProbability = _chanceOfMutationInOneMilion / 1000000.0;

