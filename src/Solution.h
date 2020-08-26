#ifndef SOLUTION_H
#define SOLUTION_H

#include "Instance.h"
#include <chrono>
#include <cstring>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#define CPU_BASE_REF 2000

using namespace std;

class Solution {
  public:
    vector< vector< int > > routes;
    Instance* instance;
    double baseTime;
    double localTime;

    Solution(Instance* instance) : instance(instance) {}
    virtual ~Solution() {}
    virtual bool parseLine(char* line) {}
    virtual bool checkSolution() {}
    virtual string getStats(std::chrono::high_resolution_clock::time_point beginTime, std::chrono::high_resolution_clock::time_point endTime, int passMark) {}
};

#endif