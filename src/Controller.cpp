#include "Controlller.h"

template < class T >
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

        std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
        file.writeStringToFile(sol.getStats(beginTime, endTime, data.passMark));

        // Duration between when started to the latest found solution.
        std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(endTime - beginTime);

        // Primal Integral is calculated based on DIMADS Challenge Rules found in http://dimacs.rutgers.edu/programs/challenge/vrp/cvrp/

        // For each solution i = 1, . . . , n, let v(i) be its value and let t(i) be the time (in seconds) it was found.
        int v_iMinus1 = this->lastSolutionCostFound.iCost;

        double t_iMinus1 = this->lastPassedTime;
        double t_i       = (ms.count() / 1000.0) * ((double)data.passMark / CPU_BASE_REF);

        // v(i-1)*(t(i) - t(i-1))/BKS*T
        this->primalIntegral += (v_iMinus1 * (t_i - t_iMinus1) / (data.baseTimeLimit * data.bestKnownSolution.iCost));

        this->lastSolutionCostFound.iCost = sol.cost;
        this->lastPassedTime              = t_i;

        // Checking if the solution is better or equal to BKS.
        if(sol.cost <= data.bestKnownSolution.iCost) {
            pclose(fp);
            break;
        }
        sol = iSolution(data.getInstance());
        fflush(stdout);
    }
    // Primal Integral is calculated based on DIMADS Challenge Rules found in http://dimacs.rutgers.edu/programs/challenge/vrp/cvrp/
    // v(n)*(T - t(n))/BKS*T
    primalIntegral += ((this->lastSolutionCostFound.iCost * (data.baseTimeLimit - lastPassedTime)) / (data.baseTimeLimit * data.bestKnownSolution.iCost));
    primalIntegral -= 1;
    primalIntegral *= 100;
    char primalIntegralString[32] = "";
    sprintf(primalIntegralString, "Primal Integral: %.3lf", primalIntegral);
    file.writeStringToFile(std::string(primalIntegralString));
}

template <>
void Controller::readStream< dSolution >(dSolution sol) {
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

        std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
        file.writeStringToFile(sol.getStats(beginTime, endTime, data.passMark));

        // Duration between when started to the latest found solution.
        std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(endTime - beginTime);

        // Primal Integral is calculated based on DIMADS Challenge Rules found in http://dimacs.rutgers.edu/programs/challenge/vrp/cvrp/

        // For each solution i = 1, . . . , n, let v(i) be its value and let t(i) be the time (in seconds) it was found.
        int v_iMinus1 = this->lastSolutionCostFound.dCost;

        double t_iMinus1 = this->lastPassedTime;
        double t_i       = (ms.count() / 1000.0) * ((double)data.passMark / CPU_BASE_REF);

        // v(i-1)*(t(i) - t(i-1))/BKS*T
        this->primalIntegral += (v_iMinus1 * (t_i - t_iMinus1) / (data.baseTimeLimit * data.bestKnownSolution.dCost));

        this->lastSolutionCostFound.dCost = sol.cost;
        this->lastPassedTime              = t_i;

        // Checking if the solution is better or equal to BKS.
        if(sol.cost == data.bestKnownSolution.dCost) {
            pclose(fp);
            break;
        }

        sol = dSolution(data.getInstance());
        fflush(stdout);
    }
    // Primal Integral is calculated based on DIMADS Challenge Rules found in http://dimacs.rutgers.edu/programs/challenge/vrp/cvrp/
    // v(n)*(T - t(n))/BKS*T
    primalIntegral += ((this->lastSolutionCostFound.dCost * (data.baseTimeLimit - lastPassedTime)) / (data.baseTimeLimit * data.bestKnownSolution.dCost));
    primalIntegral -= 1;
    primalIntegral *= 100;
    char primalIntegralString[32] = "";
    sprintf(primalIntegralString, "Primal Integral: %.3lf", primalIntegral);
    file.writeStringToFile(std::string(primalIntegralString));
}

void Controller::run() {

    // signal(SIGALRM, stopProces);
    // alarm(data.baseTimeLimit / (data.passMark / CPU_BASE_REF));
    char execCommand[PATH_MAX];
    sprintf(execCommand, "timeout %.2lfs %s", (data.baseTimeLimit / ((double)data.passMark / CPU_BASE_REF)), data.execCommand.c_str());
    // sprintf(execCommand, "%s", (data.baseTimeLimit / ((double)data.passMark / CPU_BASE_REF)), data.execCommand.c_str());

    // char execCommand[PATH_MAX];
    // sprintf(execCommand, "%s", data.execCommand.c_str());
    beginTime = std::chrono::high_resolution_clock::now();
    fp        = popen(execCommand, "r");

    if(data.isRounded) {
        iSolution sol = iSolution(data.getInstance());
        readStream(sol);
    } else {
        dSolution sol = dSolution(data.getInstance());
        readStream(sol);
    }

    file.~OutputFile();
}