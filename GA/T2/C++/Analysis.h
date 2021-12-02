#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <functional>

class Analysis {
private:
	size_t _testNo, _sampleSize, _generations;
	std::string _folderName, _name;

	static void doNothing(double);

	template<typename GAType>
	double runGA(GAType ga,const std::function<void(double)>& useVal) {
		double rv = ga.getBestOfGeneration();
		useVal(rv);
		for (size_t i = 1; i < _generations; ++i) {
			ga.nextGeneration();
			double aux = ga.getBestOfGeneration();
			useVal(aux);
			rv = std::min(rv, aux);
		}

		return rv;
	}

public:
	Analysis(const std::string& folderName, const std::string& name,  size_t sampleSize, size_t generations);

	~Analysis();

	template<typename GAType>
	void addTest(const std::string& testName, GAType ga) {
		std::ofstream fout(_folderName + "/run" + std::to_string(_testNo) + ".csv");
		fout << std::fixed << std::setprecision(5);
		fout << "gen,val," << std::endl;

		std::cout << testName << " : [";

		size_t gen = 1;
		auto writeRun = [&](double val) { fout << gen << "," << val << "," << std::endl; ++gen; };
		std::vector<double> sample;
		sample.reserve(_sampleSize);
		sample.push_back(runGA(ga, writeRun));

		fout.close();
		std::cout << "=";
		for (size_t i = 1; i < _sampleSize; ++i) {
			sample.push_back(runGA(ga, doNothing));
			std::cout << "=";
		}
		std::cout << "]" << std::endl << std::endl;


		calculate(testName, sample);

		++_testNo;
	}

	void calculate(const std::string& testName, const std::vector<double>& sample);
};