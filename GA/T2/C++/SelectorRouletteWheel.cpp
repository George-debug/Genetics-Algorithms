#include "SelectorRouletteWheel.h"

double min2Max(double x) {
	return x * (-1);
}

runInfo::Generation SelectorRouletteWheel::operator()(const runInfo::Generation& oldGeneration, size_t wantedSize) {
	runInfo::Generation rv;
	rv.reserve(wantedSize);

	double
		minimus = min2Max(oldGeneration.front().second),
		maximus = min2Max(oldGeneration.back().second),
		delta = maximus - minimus + 0.000001;

	//auto Generation2F = [=](const runInfo::Solution& x) {
	//	(maximus - x.second) / delta + 0.01;
	//};

	std::vector<double> f;
	f.reserve(oldGeneration.size());
	double fSum = 0;
	//std::transform(oldGeneration.begin(), oldGeneration.end(), std::back_inserter(f), Generation2F);
	for (const runInfo::Solution& sol : oldGeneration) {
		f.push_back((maximus - min2Max(sol.second)) / delta + 0.01);
		fSum += f.back();
	}

	std::vector<double> fn;
	fn.reserve(oldGeneration.size());

	for (double i : f)
		fn.push_back(i / fSum);

	std::vector<double>pc;
	pc.reserve(oldGeneration.size());
	
	pc.push_back(fn.front());

	for (size_t ii = 1; ii < fn.size(); ++ii)
		pc[ii] = fn[ii] + pc[ii - 1];
	pc.back() = 1;

	for (size_t ii = 0; ii < oldGeneration.size() && rv.size() < wantedSize; ++ii) {
		double r = details::getRandom01(details::s_randomDevice);
		for (size_t jj = 0; jj < pc.size(); ++jj) {
			if (r <= pc[jj]) {
				rv.push_back(oldGeneration[jj]);
				break;
			}
		}
	}

	while (rv.size() < wantedSize) {
		std::cout << "Nu am destul la Ruleta, mai pun unul" << std::endl;
		auto auxIndiv = std::make_shared<runInfo::Individual>(oldGeneration.front().first->size());
		runInfo::Solution auxSol = std::make_pair(auxIndiv, -1);

		rv.push_back(auxSol);
	}

	return rv;
}