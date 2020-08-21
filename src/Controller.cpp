#include "Controlller.h"

template<class T>
void Controller::readStream(T sol) {
    char path[PATH_MAX];
    
    while(fgets(path, PATH_MAX - 1, fp)) {
        if(!sol.parseLine(path))
            continue;

        if(sol.cost >= data.baseSolution.iCost) {
            sol = iSolution(data.getInstance());
            continue;
        }

        if(!sol.checkSolution())
            continue;

        file.writeStringToFile(sol.getStats(beginTime, data.passMark));

        if(sol.cost == data.bestKnownSolution.iCost){
            pclose(fp);
            break;
        }
        sol = iSolution(data.getInstance());
        fflush(stdout);
    }
}

template<>
void Controller::readStream<dSolution>(dSolution sol){
    char path[PATH_MAX];
    
    while(fgets(path, PATH_MAX - 1, fp)) {
        if(!sol.parseLine(path))
            continue;

        if(sol.cost >= data.baseSolution.dCost) {
            sol = dSolution(data.getInstance());
            continue;
        }

        if(!sol.checkSolution())
            continue;

        file.writeStringToFile(sol.getStats(beginTime, data.passMark));

        if(sol.cost == data.bestKnownSolution.dCost){
            pclose(fp);    
            break;
        }

        sol = dSolution(data.getInstance());    
        fflush(stdout);
    }
}

void Controller::run(){

    
    // signal(SIGALRM, stopProces);
    // alarm(data.baseTimeLimit / (data.passMark / CPU_BASE_REF));
    char execCommand[PATH_MAX];
    sprintf(execCommand, "timeout %.2lfs %s", (data.baseTimeLimit / ((double)data.passMark / CPU_BASE_REF)), data.execCommand.c_str());
    // sprintf(execCommand, "%s", (data.baseTimeLimit / ((double)data.passMark / CPU_BASE_REF)), data.execCommand.c_str());

    // char execCommand[PATH_MAX];
    // sprintf(execCommand, "%s", data.execCommand.c_str());
    beginTime = std::chrono::high_resolution_clock::now();
    fp = popen(execCommand, "r");

    if(data.isRounded) {
        iSolution sol = iSolution(data.getInstance());
        readStream(sol);
    } else {
        dSolution sol = dSolution(data.getInstance());
        readStream(sol);
    }

    file.~OutputFile();
}