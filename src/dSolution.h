#ifndef DSOLUTION_H
#define DSOLUTION_H

#include "Solution.h"

class dSolution : public Solution {

  public:
    double cost;
    dSolution(Instance* instance) : Solution(instance) {}

    bool parseLine(char* line);
    bool checkSolution();
    string getStats(std::chrono::high_resolution_clock::time_point beginTime, std::chrono::high_resolution_clock::time_point endTime, int passMark);
};

#endif