#include "geneticAlgorithm.h"

size_t arrangements(size_t m, size_t a) {
	size_t rv = 1;

	for (size_t i = 0; i < a; ++i)
		rv *= (m - i);

	return rv;
}

size_t findSelectSize(size_t populationSize, size_t inputSize, size_t outputSize) {
	size_t prev = 1, step = 16, current = prev + step;
	populationSize = static_cast<size_t>(ceil(static_cast<double>(populationSize) / outputSize));

	//std::cout << "p = " << populationSize << std::endl;

	while (step != 0) {
		current = prev + step;

		while (arrangements(current, inputSize) < populationSize) {
			//std::cout << "prev = " << prev << " || " << "current = " << current << std::endl;
			//std::cout << "arrangements(current, inputSize) = " << arrangements(current, inputSize) << std::endl;
			prev = current;
			current = prev + step;

			//std::cout << "TOOOOO:" << std::endl;
			//std::cout << "prev = " << prev << " || " << "current = " << current << std::endl << std::endl;
		}

		step /= 2;
	}

	return current;
}