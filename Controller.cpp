#include <chrono>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <signal.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>
#include "model.h"

using namespace std;
#define PATH_MAX 256
#define ARGUMENTS_COUNT 9
#define BASE 2000

FILE *fp;

void stopProces(int signal)
{
    int status = pclose(fp);
    printf("Process terminated\n");
}

bool parseLine(char *line, tSolution *sol)
{
    bool flag = false;
    char *token = strtok(line, " ");

    if (strstr(token, "Route") != NULL)
    {
        vector<int> route;
        token = strtok(NULL, " ");
        token = strtok(NULL, " ");

        while (token != NULL)
        {
            route.push_back(atoi(token));
            token = strtok(NULL, " ");
        }
        sol->routes.push_back(route);
    }
    else if (strstr(token, "Cost") != NULL)
    {
        token = strtok(NULL, " ");
        sol->cost = atof(token);
        flag = true;
    }

    return flag;
}

int main(int argc, char *argv[])
{
    tData data(argc, argv);
    tInstance instance(data.path.c_str());

    char path[PATH_MAX];

    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::milliseconds milliseconds;
    char execCommand[PATH_MAX];
    sprintf(execCommand, "timeout %.2lfs %s", (data.baseTimeLimit / (data.passMark / BASE)), data.execCommand.c_str());
    fp = popen( execCommand, "r");

    Clock::time_point t0 = Clock::now();

    tSolution sol;
    while (fgets(path, PATH_MAX, fp))
    {
        Clock::time_point t1 = Clock::now();
        milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
        if (parseLine(path, &sol)){
            if(instance.checkInstance(sol))
                cout << "Feasible Solution\n" << endl;
        }

        fflush(stdout);
    }

    return 0;
}
