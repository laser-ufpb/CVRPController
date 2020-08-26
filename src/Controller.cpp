#include "Controlller.h"

int pid;

void stopProcess(int signal) {
    kill(pid, SIGKILL);
}

template < class T >
void Controller::readStream(T sol) {
    char line[MAX_LEN];

    while(fgets(line, MAX_LEN - 1, fp)) {
        if(!sol.parseLine(line))
            continue;

        if(sol.cost >= data.baseSolution.iCost) {
            sol = iSolution(data.getInstance());
            continue;
        }

        if(!sol.checkSolution())
            continue;

        file.writeStringToFile(sol.getStats(beginTime, data.passMark));

        if(sol.cost == data.bestKnownSolution.iCost) {
            pclose(fp);
            break;
        }
        sol = iSolution(data.getInstance());
    }
}

template <>
void Controller::readStream< dSolution >(dSolution sol) {
    char line[MAX_LEN];

    while(fgets(line, MAX_LEN - 1, fp)) {
        if(!sol.parseLine(line))
            continue;

        if(sol.cost >= data.baseSolution.dCost) {
            sol = dSolution(data.getInstance());
            continue;
        }

        if(!sol.checkSolution())
            continue;

        file.writeStringToFile(sol.getStats(beginTime, data.passMark));

        if(sol.cost == data.bestKnownSolution.dCost) {
            pclose(fp);
            break;
        }

        sol = dSolution(data.getInstance());
        fflush(stdout);
    }
}

void Controller::run() {

    signal(SIGALRM, stopProcess);
    alarm(data.baseTimeLimit / (data.passMark / CPU_BASE_REF));

    this->beginTime = std::chrono::high_resolution_clock::now();

    pid = popen2(data.getExecCommandArgvs());

    if(data.isRounded) {
        iSolution sol = iSolution(data.getInstance());
        readStream(sol);
    } else {
        dSolution sol = dSolution(data.getInstance());
        readStream(sol);
    }

    kill(pid, SIGKILL);
}

int Controller::popen2(vector< char* > argvs) {
    int fd[2];
    if(pipe(fd))
        perror("Pipe failed\n");

    pid_t pid = fork();

    if(pid == 0) {
        close(fd[0]);
        if(dup2(fd[1], 1) < 0)
            perror("Duplicate stdin failed\n");

        execve(argvs[0], argvs.data(), NULL);
        exit(-1);
    } else
        close(fd[1]);

    this->fp = fdopen(fd[0], "r");

    return pid;
}
