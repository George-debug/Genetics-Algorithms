#include <iostream>
#include <ratio>
#include <cmath>
#include <limits>
#include <algorithm>
#include <bitset> //!!
#include <iomanip>
#include <fstream>
#include <string>
#include <direct.h>

#include "BinaryChromosome.h"
#include "GeneticAlgorithm.h"
#include "Selector.h"
#include "SelectorRouletteWheel.h"
#include "CrossoverKPoint.h"
#include "details.h"
#include "runInfo.h"
#include "Analysis.h"

//#define precition 5

const double pi = 3.14159265359;

class FunctionFitness {
    const double c_lowLimit, c_delta, c_maxVal;
    runInfo::FunctionFitness _fitness;
    std::function<double(UIntType)> _converter;

    double UInt2Double(UIntType val) const {
        double dVal = static_cast<double>(val);

        double output = dVal / c_maxVal * c_delta + c_lowLimit;

        //std::cout
        //    << "UInt2Double: " << std::endl
        //    << "val = " << val << std::endl
        //    << "dVal = " << dVal << std::endl
        //    << "c_maxVal = " << c_maxVal << std::endl
        //    << "c_delta = " << c_delta << std::endl
        //    << "c_lowLimit = " << c_lowLimit << std::endl
        //    << "output = " << output << std::endl << std::endl << std::endl;


        return output;
    }

public:
    
    FunctionFitness(double lowLimit, double upperLimit, runInfo::FunctionFitness fitness)
        : c_lowLimit(lowLimit), c_delta(upperLimit - lowLimit),
        c_maxVal(pow(2, runInfo::bitSize)-1),
        _fitness(fitness)
    {
        _converter = [=](UIntType val) {return UInt2Double(val); };
    }

    double operator()(const runInfo::Individual& val) const {
        std::vector<double> output;
        output.reserve(val.size());

        std::transform(val.begin(), val.end(), std::back_inserter(output), _converter);

        return _fitness(output);
    }
};

double deJong(const std::vector<double>& x) {
    double rv = 0;
    for (auto i : x)
        rv += i * i;

    return rv;
}

double schwefel(const std::vector<double>& x) {
    double rv = 0;
    for (auto i : x)
        rv += -i * sin(sqrt(abs(i)));

    return rv;
}

double michalewicz(const std::vector<double>& x)
{
    double rv = 0, m = 10, dim = x.size();
    for (int i = 0; i < dim; i++)
        rv += sin(x[i]) * pow((sin((double(i) + 1) * x[i] * x[i] / pi)), (2 * m));
    return -rv;
}

double rastigin(const std::vector<double>& x)
{
    long double rv = 10 * (double)x.size();
    for (auto i : x)
        rv += i * i - 10 * cos(2 * pi * i);
    return rv;
}

void printGeneration(const runInfo::Generation& gen) {
    std::cout << gen[0].second << std::endl;
}


//#========================================================================================#

std::vector<size_t> makeEvenCuts(size_t parts, size_t dimensions) {
    std::vector<size_t> rv;
    rv.reserve(parts);
    size_t allBits = dimensions * runInfo::bitSize;

    size_t chunk = allBits / parts, currentChunk = 0;

    for (size_t i = 1; i < parts; ++i) {
        currentChunk += chunk;
        rv.push_back(currentChunk);
    }

    return rv;
}

void makeFolder(const std::string& path) {
    int s = _mkdir(path.c_str());

    if (s == -1)
        std::cout << "We couldn't create Folder: " << path << std::endl;
}

//#========================================================================================#

int main()
{
    std::vector<size_t> dimensionVector = { 30, 10, 5 };
    //std::vector<std::pair<std::string, std::shared_ptr<FunctionFitness>>> functionVector = {
    //    { "michalewicz", std::make_shared<FunctionFitness>(0, pi, michalewicz) },
    //    { "rastrigin", std::make_shared<FunctionFitness>(-5.12, 5.12, rastigin) },
    //    { "dejong", std::make_shared<FunctionFitness>(-5.12, 5.12, deJong) }
    //    
    //};

    std::vector<std::pair<std::string, std::shared_ptr<FunctionFitness>>> functionVector = {
        { "Schwefel", std::make_shared<FunctionFitness>(-500, 500, schwefel) }
    };


    std::vector<std::pair<std::string, std::shared_ptr<Selector>>> SelectorVector = {
        {"Truncation Selection", std::make_shared<Selector>()},
        {"Roulette Wheel Selection", std::make_shared<SelectorRouletteWheel>()}
    };

    auto chromosome = std::make_shared<runInfo::Chromosome>();


    for (auto s : SelectorVector) {
        std::string mainFolder = s.first;
        makeFolder(mainFolder);


     

        for (auto func : functionVector) {
            std::cout << std::fixed << std::setprecision(5);


            std::string functionFolder = mainFolder + "/" + func.first;
            makeFolder(functionFolder);

            for (size_t dimensions : dimensionVector) {
                std::string dimensionFolder = functionFolder + "/" + std::to_string(dimensions);
                makeFolder(dimensionFolder);

                std::cout
                    << func.first << " : " << dimensions << std::endl;
                
                Analysis analysis(dimensionFolder, func.first + " - " + std::to_string(dimensions) + "dimensions - " + s.first, runInfo::numberOfTests, runInfo::iterations);

                for (size_t i = 2; i < 6 ; ++i) {
                    std::vector<size_t> evenCuts = makeEvenCuts(i, dimensions);
                    //std::cout << evenCuts.size() << std::endl;
                    auto crossover = std::make_shared<CrossoverKPoint>(evenCuts);

                    GeneticAlgorithm<FunctionFitness> ga(runInfo::popSize,
                        dimensions,
                        s.second,
                        crossover,
                        chromosome,
                        *func.second);
                    analysis.addTest(std::to_string(i-1) + " Points", ga);
                }
                
            }
        }
    }
    //std::vector<size_t> points = { 120 };
    //std::shared_ptr<Crossover> cross1 = std::make_shared<CrossoverKPoint>(points);

    //geneticAlgorithm(300, 30, selector1, cross1, ch, ff, printGeneration);


    std::cin.get();

}
