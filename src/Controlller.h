#ifndef CONTROLLER_H
#define CONTROLLER_h

#include "Data.h"
#include "OutputFile.h"
#include "dSolution.h"
#include "iSolution.h"
#include <chrono>
#include <iostream>
#include <signal.h>
#define PATH_MAX     256
#define CPU_BASE_REF 2000

class Controller {
    Data data;
    OutputFile file;
    std::chrono::high_resolution_clock::time_point beginTime;
    FILE* fp;
    template < class T >
    void readStream(T sol);

    double_t primalIntegral;
    // Equivalent to t(i-1)
    double lastPassedTime;
    // Equivalent to v(i-1)
    Cost lastSolutionCostFound;

  public:
    Controller() {}
    Controller(int argc, char* argv[]) : data(Data(argc, argv)), file(OutputFile(data.getNameOfOutputFile())) {
        string header;
        this->lastPassedTime        = 0;
        this->primalIntegral        = 0;
        this->lastSolutionCostFound = data.baseSolution;
        try {
            header = data.createHeader();
            file.writeStringToFile(header);
        } catch(const char* e) {
            std::cout << e << '\n';
        }
    }

    void run();
};

#endif