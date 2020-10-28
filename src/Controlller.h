#ifndef CONTROLLER_H
#define CONTROLLER_h

#include "Data.h"
#include "OutputFile.h"
#include "dSolution.h"
#include "iSolution.h"
#include <chrono>
#include <iostream>
#include <signal.h>
#include <unistd.h>

// #define MAX_LEN      170000
#define CPU_BASE_REF 2000

static int isAlarmSet = 0;

class Controller {
    Data data;
    OutputFile file;
    std::chrono::high_resolution_clock::time_point beginTime;
    FILE* fp;
    template < class T >
    void readStdoutFromChildProcess(T sol);

    double_t primalIntegral;
    // Equivalent to t(i-1)
    double lastPassedTime;
    // Equivalent to v(i-1)
    double lastSolutionCostFound;
    // Max number of characteres in line
    int MAX_LEN;
  public:
    Controller() {}
    Controller(int argc, char* argv[]);

    // Run competitor's program
    void run();
    // Open child process
    int popen2(vector< char* > argvs);
};

#endif