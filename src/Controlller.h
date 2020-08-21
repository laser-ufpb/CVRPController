#ifndef CONTROLLER_H
#define CONTROLLER_h

#include "Data.h"
#include "OutputFile.h"
#include "iSolution.h"
#include "dSolution.h"
#include <chrono>
#include <signal.h>
#include <iostream>
#define PATH_MAX     256
#define CPU_BASE_REF 2000

class Controller{
    Data data;
    OutputFile file;
    std::chrono::high_resolution_clock::time_point beginTime;
    FILE * fp;

    template< class T>
    void readStream(T sol);

    public:
        Controller(){}
        Controller(int argc, char* argv[]) : data(Data(argc,argv)), file(OutputFile(data.getNameOfOutputFile())){
            string header;
            try
            {
                header = data.createHeader();
                file.writeStringToFile(header);
            }
            catch(const char * e)
            {
                std::cout << e << '\n';
            }   
        }

        void run();
};

#endif