#include "runInfo.h"
#include "BinaryChromosome.h"


namespace runInfo {
	bool equalDouble(double a, double b) {
		return abs(a - b) < myEpsilon;
	}

	bool SolutionEqual(const Solution& x, const Solution& y) {
		//std::cout << "We used your SOLUTION '==' " << std::endl;
		if (!equalDouble(x.second, y.second))
			return false;

		size_t individualSize = x.first->size();
		for (size_t i = 0; i < individualSize; ++i) {
			for (int j = bitSize - 1; j >= 0; j--) {
				if (x.first->at(i)._representation[j] != y.first->at(i)._representation[j])
					return false;
			}
		}
		return true;
	}

	bool SolutionNotEqual(const Solution& x, const Solution& y) {
		//std::cout << "We used your SOLUTION '!=' " << std::endl;
		//i have no idea if is auto :)
		return !(x == y);
	}

	bool SolutionSmallerThen(const Solution& x, const Solution& y)
	{
		//std::cout << "We used your SOLUTION '<' " << std::endl;
		if (!equalDouble(x.second, y.second))
			return x.second < y.second;

		//std::cout << "nu sunt egale" << std::endl;

		size_t individualSize = x.first->size();
		for (size_t i = 0; i < individualSize; ++i) {
			for (int j = bitSize - 1; j >= 0; j--) {
				if (x.first->at(i)._representation[j] ^ y.first->at(i)._representation[j])
					return y.first->at(i)._representation[j];
			}
		}
		return false;
	}

	bool insertSorted(Generation& v, const Solution& n)
	{
		//std::cout << "using back inserter" << std::endl;
		auto insert_itr = std::lower_bound(std::begin(v), std::end(v), n, SolutionSmallerThen);

		if (insert_itr == std::end(v) || SolutionNotEqual(*insert_itr, n))
		{
			v.insert(insert_itr, n);
			return true;
		}

		return false;
	}

	void printGeneration(const runInfo::Generation& g) {
		for (const runInfo::Solution& s : g) {
			std::cout << s.second << " <= ";

			for (const runInfo::Chromosome& i : *(s.first)) {
				std::cout << static_cast<UIntType>(i) << " ";
			}

			std::cout << " || ";

			for (const runInfo::Chromosome& i : *(s.first)) {
				for (int bIndex = runInfo::bitSize - 1; bIndex > 0; --bIndex)
					std::cout << i._representation[bIndex];
				std::cout << "  ";
			}
			std::cout << std::endl;
		}
	}

	void sortGeneration(Generation& gen) {
		std::sort(gen.begin(), gen.end(), SolutionSmallerThen);
	}
}