#include <iostream>
#include <iomanip>

#include "BinaryVector.h"
#include "GlobalOptima.h"
#include "MultidimensionalVector.h"
#include "HillClimbing.h"
#include "SimulatedAnnealing.h"

const double pi = 3.14159265359;

double deJong(const std::vector<double>& x) {
    double rv = 0;
    for (auto i : x)
        rv += i * i;

    return rv;
}

double schwefel(const std::vector<double>& x) {
    double rv = 0;
    for (auto i : x)
        rv += -i*sin(sqrt(abs(i)));

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
    for(auto i : x)
        rv += i * i - 10 * cos(2 * pi * i);
    return rv;
}

#define decimals 5

struct Result {
    double _time, _minVal;

    void change(double time, double val) {
        //std::cout << time << " | " << val << std::endl;
        _time = time;
        _minVal = val;
    }
};

struct FunctionDetails {
    std::string _name;
    double(*_funcPtr)(const std::vector<double>&);
    double _lowLimit, _upperLimit;

    FunctionDetails(const std::string name, double(*funcPtr)(const std::vector<double>&), double lowLimit, double upperLimit)
        : _name(name), _funcPtr(funcPtr), _lowLimit(lowLimit), _upperLimit(upperLimit)
    {}
};


Result* currentResult;

void changeResult(double time, double val) {
    currentResult->change(time, val);
}

bool compareByTime(const Result& a, const Result& b) {
    return a._time < b._time;
}

bool compareByVal(const Result& a, const Result& b) {
    return a._minVal < b._minVal;
}

double getAverageTime(const std::vector<Result>& x) {
    double rv = 0;
    for (const Result& i : x) {
        rv += i._time;
    }
    return rv/x.size();
}

double getAverageVal(const std::vector<Result>& x) {
    double rv = 0;
    for (const Result& i : x) {
        rv += i._minVal;
    }
    return rv / x.size();
}

int main()
{
    
    std::cout << "Rastrigin:" << std::endl << std::endl;
    std::cout << std::setprecision(6) << std::fixed;

    //Result x;
    //currentResult = &x;
    //std::shared_ptr<MultidimensionalVector<double>> v = std::make_shared<BinaryVector<uint32_t, double>>(5, -5.12, 5.12);
    //std::shared_ptr<GlobalOptima<double, double>> go = std::make_shared<HillClimbing<double, double>>(v.get(), 1000, HillClimbing<double, double>::firstImprovement);

    //go->run(deJong, changeResult);

    //std::cout << ">> " << x._time << " | " << x._minVal << std::endl;

    std::pair<double, double> a;
    size_t iterations = 30;
    std::vector<size_t> dimensions = { 5, 10, 30 };
    std::vector<FunctionDetails> functions;
    functions.reserve(4);
    functions.emplace_back("DeJong", deJong, -5.12, 5.12);
    functions.emplace_back("Schwefel", schwefel, -500, 500);
    functions.emplace_back("Michalewicz", michalewicz, 0, pi);
    functions.emplace_back("Rastigin", rastigin, -5.12, 5.12);


    for (auto func : functions) {
        for (auto d : dimensions) {
            std::shared_ptr<MultidimensionalVector<double>> x = std::make_shared<BinaryVector<uint32_t, double>>(d, func._lowLimit, func._upperLimit);
            
            std::vector<
                std::pair<
                    std::string, std::shared_ptr<GlobalOptima<double, double>>
                >
            > algorithms = {
                {"Hill Climbing First Improvement", std::make_shared<HillClimbing<double, double>>(x.get(), 1000, HillClimbing<double, double>::firstImprovement)},
                {"Hill Climbing Best Improvement", std::make_shared<HillClimbing<double, double>>(x.get(), 1000, HillClimbing<double, double>::bestImprovement)},
                {"Simulated Annealing", std::make_shared<SimulatedAnnealing<double, double>>(x.get(), 0.4, 0.001)}
            };

            for (auto algo : algorithms) {
                std::vector<Result> results(iterations);

                double bestTime, averageTime, worstTime;
                double bestVal, averageVal, worstVal;
                std::cout << "Loading: ";
                for (Result& r : results) {
                    currentResult = &r;
                    algo.second->run(func._funcPtr, changeResult);
                    std::cout << '.';
                }

                std::cout << std::endl;

                bestTime = std::min_element(results.begin(), results.end(), compareByTime)->_time;
                worstTime = std::max_element(results.begin(), results.end(), compareByTime)->_time;
                averageTime = getAverageTime(results);

                bestVal = std::min_element(results.begin(), results.end(), compareByVal)->_minVal;
                worstVal = std::max_element(results.begin(), results.end(), compareByVal)->_minVal;
                averageVal = getAverageVal(results);

                std::cout
                    << func._name << " : " << d << std::endl
                    << algo.first << ":" << std::endl << std::endl
                    << "Best Time: " << bestTime << std::endl
                    << "Worst Time: " << worstTime << std::endl
                    << "Average Time: " << averageTime << std::endl << std::endl
                    << "Best Val: " << bestVal << std::endl
                    << "Worst Val: " << worstVal << std::endl
                    << "Average Val: " << averageVal << std::endl
                    << "=====================" << std::endl << std::endl << std::endl;
            }
        }
    }

    std::cin.get();
    int tbs;
    std::cin >> tbs;
    

    ////SimulatedAnnealing<double, double> sa(printVars, x, 0.4, 0.001);
    //HillClimbing<double, double> hc(printVars, x, 1000, HillClimbing<double, double>::firstImprovement);

    //hc.run(michalewicz);

}
