#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>

long long int f(unsigned char var) {
    long long int x = (long long int) var;

    return x * x * x - 60 * x * x + 900 * x + 100;
}
//{ 1, 2, 4, 8, 16 } ; {16, 8, 4, 2, 1}
unsigned char everyBitOn[] = { 16, 8, 4, 2, 1 };

//std::vector< std::pair<size_t, unsigned char> > nodeValues(32, { -1, 0 });

unsigned char firstImprovement(unsigned char var) {
    long long int value = f(var);

    //std::cout << "var = " << int(var) << std::endl;

    for (size_t i = 0; i < 5; ++i) {
        unsigned char newVar = var ^ everyBitOn[i];

        //std::cout << "newVar = " << int(newVar) << std::endl;

        if (f(newVar) > value)
            return newVar;
    }

    return var;
}

unsigned char bestImprovement(unsigned char var) {
    unsigned char max = var;

    for (size_t i = 0; i < 5; ++i) {
        unsigned char newVar = var ^ everyBitOn[i];

        if (f(newVar) > f(max))
            max = newVar;
    }

    return max;
}

unsigned char (*improvementUsed)(unsigned char var) = bestImprovement;

unsigned char getMaxImprovement(unsigned char var) {
    unsigned char impr = improvementUsed(var);
    
    while (var != impr) {
        var = impr;
        impr = improvementUsed(impr);
    }

    return impr;
}

size_t getStepsCounterUntilMaxImprovement(unsigned char var) {
    unsigned char impr = improvementUsed(var);
    size_t rv = 0;

    while (var != impr) {
        var = impr;
        impr = improvementUsed(impr);
        ++rv;
    }

    return rv;
}

//void getNoteValue(unsigned char var) {
//    if (nodeValues[var].first != -1)
//        return ;
//
//    unsigned char nextVal = improvementUsed(var);
//    if (var == nextVal) {
//        nodeValues[var].first = 0;
//        nodeValues[var].second = var;
//    }
//
//
//
//}

void printTikz() {
    std::cout << "\\begin{tikzpicture}" << std::endl;
    std::cout << "\\begin{pgfonlayer}{nodelayer}" << std::endl;
    for (unsigned char i = 0; i < 32; ++i) {
        std::cout << "\\node [style={simple_node}] (" << int(i) << ") at (0, 0) {" << int(i) << "};" << std::endl;
    }
    std::cout << "\\end{pgfonlayer}" << std::endl;
    std::cout << "\\begin{pgfonlayer}{edgelayer}" << std::endl;
    for (unsigned char i = 0; i < 32; ++i) {
        unsigned char impr = improvementUsed(i);
        if (impr != i)
            std::cout << "\\draw [style={simple_line}] (" << int(i) << ") to (" << int(impr) << "); " << std::endl;
    }
    std::cout << "\\end{pgfonlayer}" << std::endl;
    std::cout << "\\end{tikzpicture}" << std::endl;
}

void printData() {
    std::vector<std::vector<unsigned char>> list(32), filtered;

    for (unsigned char i = 0; i < 32; ++i) {
        auto impr = getMaxImprovement(i);

        list[impr].push_back(i);
        std::cout << int(i) << " -- " << int(impr) << std::endl;
    }

    std::copy_if(list.begin(), list.end(), std::back_inserter(filtered), [](const std::vector<unsigned char>& i) {return i.size() > 0; });

    for (auto& i : filtered) {
        auto impr = getMaxImprovement(i[0]);
        std::cout << "x" << int(impr) << ", y" << int(impr) << ", ";
    }
    std::cout << std::endl;

    while (true) {
        bool ok = true;

        for (auto& i : filtered) {
            if (i.size() > 0) {
                ok = false;
                std::cout << int(i.back()) << ", " << f(i.back()) << ", ";
                i.pop_back();
            }
            else {
                std::cout << ", , " ;
            }
        }

        std::cout << std::endl;


        if (ok) break;
    }
}

void averageLevel() {
    size_t total = 0;

    for (unsigned char i = 0; i < 32; ++i) {
        total += getStepsCounterUntilMaxImprovement(i);
    }

    std::cout << double(total) / 32 << std::endl;
}

int main()
{
    //printTikz();
    //printData();
    averageLevel();
    return 0;
}
