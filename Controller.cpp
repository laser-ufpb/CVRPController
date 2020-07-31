#include <chrono>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <signal.h>
#include <unistd.h>
#include <string>
#include <iostream>
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

struct tData
{
    string competitorName;
    string path;
    string execCommand;
    double baseTimeLimit;
    double baseSolution;
    double bestSolution;
    int isOptimal;
    int isRounded;
    int passMark;

    tData(int argc, char *arguments[])
    {
        competitorName = string(arguments[1]);
        path = string(arguments[2]);
        isRounded = atoi(arguments[3]);
        passMark = atoi(arguments[4]);
        baseTimeLimit = atof(arguments[5]);
        baseSolution = atof(arguments[6]);
        bestSolution = atof(arguments[7]);
        isOptimal = atoi(arguments[8]);
        execCommand = string(arguments[9]);
        for (int i = 10; i < argc; i++)
        {
            execCommand += " " + string(arguments[i]);
        }
    }

};

int main(int argc, char *argv[])
{
    tData data = tData(argc, argv);
    char path[PATH_MAX];
    
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::milliseconds milliseconds;

    signal(SIGALRM, stopProces);
    alarm(data.baseTimeLimit/(data.passMark/BASE));
    // char * command = data.execCommand.c_str();
    fp = popen(data.execCommand.c_str(), "r");
    if (fp == NULL);

    Clock::time_point t0 = Clock::now();
    int i;
    while (fgets(path, PATH_MAX, fp) != NULL)
    {
        sscanf(path,"solution: %d",&i);
        Clock::time_point t1 = Clock::now();
        milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
        printf("%s | in %f ms\n", path, (float)ms.count() / 1000);
        fflush(stdout);
    }

    return 0;
}
