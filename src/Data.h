#ifndef DATA_H
#define DATA_H
#include "Instance.h"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
#define LEN          256
#define CPU_BASE_REF 2000

using namespace std;

class Data {
    // Get date and time
    string getTime();
    // Get competior's OS
    string getOS();
    // Get competitor's CPU stats
    string getCpuStats();
    vector< string > argvs;

  public:
    int isOptimal;
    int isRounded;
    int passMark;
    double baseSolution;
    double bestKnownSolution;
    double baseTimeLimit;
    string competitorName;
    string path;
    string execCommand;
    Instance instance;

    Data() {}
    Data(int argc, char* arguments[]);
    Instance* getInstance() { return &instance; }
    // Create header with data from competitor, compertitor's machine, date
    string createHeader();
    // Get name of output file
    string getNameOfOutputFile();
    // Separate string with command to run competitor's program by space
    void separateExecCommands();
    // Get competitor's program commands separated
    vector< char* > getExecCommandArgvs();
};

#endif