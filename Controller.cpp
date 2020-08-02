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
#define PATH_MAX        256
#define ARGUMENTS_COUNT 9
#define CPU_BASE_REF    2000

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
        puts("ERROR: I can't open /proc/cpuinfo. Maybe not running Linux.");
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
void printOutputHeader(tData& data, tInstance& instance) {
    std::locale::global(std::locale("en_US.UTF-8"));
    std::time_t t;
    std::time(&t);
    std::tm* tm = std::localtime(&t);

    char* string_time = new char[64];
    if(!std::strftime(string_time, 64, "%c", tm)) {
        std::cout << "DEU RUIM CARALHO" << std::endl;
        _exit(-1);
    };

    char* string_os = new char[128];
    FILE* fp        = popen("lsb_release -ds", "r");
    if(!fp) {
        puts("ERROR: I can't open process lsb_realease. Maybe not running Linux.");
        _exit(-1);
    }
    fgets(string_os, 128, fp);
    pclose(fp);

    char* string_cpu = getCpuInfo();
    printf(
        "12th DIMACS Implementation Challenge: Vehicle Routing \n"
        "CVRP track                                            \n"
        "Competitor: %s                                        \n"
        "%s                                                    \n"
        "%s"
        "%s"
        "PassMark Single Thread Benchmark: %d                  \n"
        "Time factor: %.2lf (baseline %d)                      \n"
        "Instance: %s                                          \n"
        "EUC_2D distances rounded: %d                          \n"
        "Standardized Time limit: %.0lf secs                   \n"
        "Local Machine Time Limit: %.2lf secs                  \n"
        "Base solution: %.2lf                                  \n"
        "BKS: %.2lf                                            \n"
        "Optimal: %d                                           \n",
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
        data.bestSolution,
        data.isOptimal);

    delete string_time;
    delete string_os;
    free(string_cpu - 13);
}

int main(int argc, char* argv[]) {
    tData data(argc, argv);
    tInstance instance(data.path.c_str(), data.isRounded);

    char path[PATH_MAX];

    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::milliseconds milliseconds;
    char execCommand[PATH_MAX];
    sprintf(execCommand, "timeout %.2lfs %s", (data.baseTimeLimit / (data.passMark / CPU_BASE_REF)), data.execCommand.c_str());
    fp = popen(execCommand, "r");

    Clock::time_point t0 = Clock::now();
    tSolution sol;
    printOutputHeader(data, instance);
    while(fgets(path, PATH_MAX, fp)) {
        Clock::time_point t1 = Clock::now();
        milliseconds ms      = std::chrono::duration_cast< milliseconds >(t1 - t0);
        if(parseLine(path, &sol)) {
            if(instance.checkInstance(sol))
                cout << sol.cost
                     << endl;
        }

        fflush(stdout);
    }

    return 0;
}
