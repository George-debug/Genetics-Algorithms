#include "Analysis.h"

#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <string>

Analysis::Analysis(const std::string& folderName, const std::string& name, size_t sampleSize, size_t generations)
	: _testNo(1), _folderName(folderName), _sampleSize(sampleSize), _generations(generations), _name(name)
{}

void Analysis::doNothing(double){}

void Analysis::calculate(const std::string& testName, const std::vector<double>& sample) {

	std::ofstream foutSample(_folderName + "/sample" + std::to_string(_testNo) + ".dat");
	foutSample << std::fixed << std::setprecision(5);

	for (double i : sample)
		foutSample << i << std::endl;

	foutSample.close();

	double
		minimum = std::numeric_limits<double>::max(),
		maximum = minimum * -1,
		mean = -73,
		standardDeviation = 0,
		marginOfError = -73,
		sum = 0;

	//std::cout << minimum << " " << maximum << std::endl;

	for (double i : sample) {
		sum += i;
		minimum = std::min(minimum, i);
		maximum = std::max(maximum, i);
	}

	//std::cout << minimum << " " << maximum << std::endl;

	mean = sum / _sampleSize;

	for (double i : sample) {
		double aux = i - mean;
		standardDeviation += aux * aux;
	}
	standardDeviation /= _sampleSize;
	standardDeviation = sqrt(standardDeviation);

	marginOfError = standardDeviation / sqrt(static_cast<double>(_sampleSize));

	std::ofstream fout(_folderName + "/info" + std::to_string(_testNo) + ".dat");
	fout << std::fixed << std::setprecision(5);


	fout << "mean: " << std::endl << mean << std::endl << std::endl;
	fout << "marginOfError: " << std::endl << marginOfError << std::endl << std::endl;
	fout << "best: " << std::endl << minimum << std::endl << std::endl;
	fout << "worst: " << std::endl << maximum << std::endl << std::endl;

	fout.close();
}

Analysis::~Analysis() {
	std::ofstream fout(_folderName + "/testInfo.dat");

	fout
		<< _name << std::endl
		<< _testNo << std::endl;

	fout.close();
}