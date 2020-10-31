#ifndef DSOLUTION_H
#define DSOLUTION_H

#include "Solution.h"

class dSolution : public Solution {

  public:
    double cost;
    dSolution(Instance* instance) : Solution(instance) {}

    double round(double value) {
      double v = (int) (value * 100 + .5);
      return (double) v/100;
    }

    bool parseLine(char* line);
    bool checkSolution();
    string getStats(std::chrono::high_resolution_clock::time_point beginTime, std::chrono::high_resolution_clock::time_point endTime, int passMark);
};

#endif