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

#define MAX_LEN      256
#define CPU_BASE_REF 2000

static int isAlarmSet = 0;

class Controller {
    Data data;
    OutputFile file;
    std::chrono::high_resolution_clock::time_point beginTime;
    FILE* fp;

    template < class T >
    void readStream(T sol);

  public:
    Controller() {}
    Controller(int argc, char* argv[]) : data(Data(argc, argv)), file(OutputFile(data.getNameOfOutputFile())) {
        string header;
        try {
            header = data.createHeader();
            file.writeStringToFile(header);
        } catch(const char* e) {
            std::cout << e << '\n';
        }
    }

    void run();
    int popen2(vector< char* > argvs);
};

#endif