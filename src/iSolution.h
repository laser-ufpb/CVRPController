#ifndef ISOLUTION_H
#define ISOLUTION_H

#include "Solution.h"

class iSolution : public Solution {

  public:
    int cost;

    iSolution(Instance* instance) : Solution(instance) {}

    bool parseLine(char* line);
    bool checkSolution();
    string getStats(std::chrono::high_resolution_clock::time_point beginTime, int passMark);
};

#endif