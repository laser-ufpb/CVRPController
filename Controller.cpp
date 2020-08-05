#include "model.h"
#include <chrono>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <signal.h>
#include <string>
#include <time.h>
#include <unistd.h>
#include <vector>

using namespace std;
#define PATH_MAX     256
#define CPU_BASE_REF 2000

FILE* fp;

void stopProces(int signal) {
    int status = pclose(fp);
    printf("Process terminated\n");
}

bool parseLine(char* line, tSolution* sol) {
    bool flag   = false;
    char* token = strtok(line, " ");

    if(strstr(token, "Route") != NULL) {
        vector< int > route;
        token = strtok(NULL, " ");
        token = strtok(NULL, " ");

        while(token != NULL) {
            route.push_back(atoi(token));
            token = strtok(NULL, " ");
        }
        sol->routes.push_back(route);
    } else if(strstr(token, "Cost") != NULL) {
        token     = strtok(NULL, " ");
        sol->cost = atof(token);
        flag      = true;
    }

    return flag;
}
char* getCpuInfo() {
    FILE* cpuinfo = fopen("/proc/cpuinfo", "rb");

    if(!cpuinfo) {
        puts("ERROR: I can't open /proc/cpuinfo. Maybe not running on Linux.");
        _exit(-1);
    }
    char* line_start = (char*)malloc(64 * sizeof(char));
    size_t size      = 0;
    while(getdelim(&line_start, &size, '\n', cpuinfo) != -1) {
        if(!std::strncmp("model name", line_start, 10)) {
            break;
        }
    }
    fclose(cpuinfo);
    return (line_start + 13);
}

//  Solutions is set v and t, so v(0) = solutions[0].cost and t[0] = solutions[0].localTime;
void writeOutput(tData& data, tInstance& instance, vector< tSolution > solutions) {
    std::locale::global(std::locale("en_US.UTF-8"));
    std::time_t t;
    std::time(&t);
    std::tm* tm = std::localtime(&t);

    char* string_time = new char[64];
    if(!std::strftime(string_time, 64, "%c", tm)) {
        std::cout << "ERROR" << std::endl;
        _exit(-1);
    };

    char* string_os = new char[128];
    FILE* fp        = popen("lsb_release -ds", "r");
    if(!fp) {
        puts("ERROR: I can't open process lsb_realease. Maybe not running on Linux.");
        _exit(-1);
    }
    fgets(string_os, 128, fp);
    pclose(fp);

    char* string_cpu = getCpuInfo();

    string outputName = "DIMACS-CVRP";
    outputName += "-" + data.competitorName + "-" + instance.instanceName + ".out";
    FILE* output = fopen(outputName.c_str(), "w");

    // HEADER INFO
    fprintf(output,
            "12th DIMACS Implementation Challenge: Vehicle Routing\n"
            "CVRP track\n"
            "Competitor: %s\n"
            "%s\n"
            "%s"
            "%s"
            "PassMark Single Thread Benchmark: %d\n"
            "Time factor: %.2lf (baseline %d)\n"
            "Instance: %s\n"
            "EUC_2D distances rounded: %d\n"
            "Standardized Time limit: %.0lf secs\n"
            "Local Machine Time Limit: %.2lf secs\n"
            "Base solution: %.2lf\n"
            "BKS: %.2lf\n"
            "Optimal: %d\n",
            data.competitorName.c_str(),
            string_time,
            string_cpu,
            string_os,
            data.passMark,
            (data.passMark / (double)CPU_BASE_REF), CPU_BASE_REF,
            instance.instanceName.c_str(),
            data.isRounded,
            data.baseTimeLimit,
            (data.baseTimeLimit / (data.passMark / (double)CPU_BASE_REF)),
            data.baseSolution,
            data.bestKnownSolution,
            data.isOptimal);

    // SOLVER OUTPUT
    for(int i = 1; i < solutions.size(); i++) {
        fprintf(output, "%.3lf %.3lf %.3lf\n", solutions[i].cost, solutions[i].localTime, solutions[i].baseTime);
    }

    // DATE
    std::time(&t);
    tm = std::localtime(&t);
    if(!std::strftime(string_time, 64, "%c", tm)) {
        std::cout << "ERROR" << std::endl;
        _exit(-1);
    };
    fprintf(output, "%s\n", string_time);

    // PRIMAL INTEGRAL
    int n                 = solutions.size() - 1;
    double primalIntegral = 0;
    for(int i = 1; i < solutions.size(); i++) {
        // v(i-1)*(t(i)-t(i-1))
        primalIntegral += (solutions[i - 1].cost * (solutions[i].baseTime - solutions[i - 1].baseTime));

        // * Maybe i should move this out.
        // v(n)*(T-t(n))to
        primalIntegral += (solutions[n].cost * (data.baseTimeLimit - solutions[n].baseTime));
    }
    primalIntegral /= (data.baseTimeLimit * data.bestKnownSolution);
    primalIntegral -= 1;
    primalIntegral *= 100;
    fprintf(output, "Primal Integral: %lf\n", primalIntegral);
    fclose(output);
    delete string_time;
    delete string_os;
    free(string_cpu - 13);
}

int main(int argc, char* argv[]) {
    tData data(argc, argv);
    tInstance instance(data.path.c_str(), data.isRounded);
    vector< tSolution > solutions = {{{}, data.bestKnownSolution * 1.1, 0, 0}};
    // solutions.push_back(tSolution(data.baseSolution, 0.0, 0.0));

    char path[PATH_MAX];

    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::milliseconds milliseconds;
    char execCommand[PATH_MAX];
    sprintf(execCommand, "timeout %.2lfs %s", (data.baseTimeLimit / ((double)data.passMark / CPU_BASE_REF)), data.execCommand.c_str());
    fp = popen(execCommand, "r");

    Clock::time_point t0 = Clock::now();
    tSolution sol;

    while(fgets(path, PATH_MAX, fp)) {

        if(!parseLine(path, &sol))
            continue;

        if(!instance.checkInstance(sol))
            continue;

        Clock::time_point t1 = Clock::now();
        milliseconds ms      = std::chrono::duration_cast< milliseconds >(t1 - t0);
        sol.baseTime         = (ms.count() / 1000.0) * ((double)data.passMark / CPU_BASE_REF);
        sol.localTime        = ms.count() / 1000.0;

        solutions.push_back(sol);

        if(sol.cost == data.bestKnownSolution)
            pclose(fp);

        fflush(stdout);
    }

    writeOutput(data, instance, solutions);

    return 0;
}
