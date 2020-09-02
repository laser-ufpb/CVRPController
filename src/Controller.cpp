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

        if(sol.cost >= data.baseSolution) {
            sol = T(data.getInstance());
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
        double v_iMinus1 = this->lastSolutionCostFound;

        double t_iMinus1 = this->lastPassedTime;
        double t_i       = (ms.count() / 1000.0) * ((double)data.passMark / CPU_BASE_REF);

        // v(i-1)*(t(i) - t(i-1))/BKS*T
        this->primalIntegral += (v_iMinus1 * (t_i - t_iMinus1) / (data.baseTimeLimit * data.bestKnownSolution));

        this->lastSolutionCostFound = sol.cost;
        this->lastPassedTime        = t_i;

        // Checking if the solution is better or equal to BKS.
        if(sol.cost <= data.bestKnownSolution) {
            pclose(fp);
            break;
        }
        sol = T(data.getInstance());
    }
    // Primal Integral is calculated based on DIMADS Challenge Rules found in http://dimacs.rutgers.edu/programs/challenge/vrp/cvrp/
    // v(n)*(T - t(n))/BKS*T
    primalIntegral += ((this->lastSolutionCostFound * (data.baseTimeLimit - lastPassedTime)) / (data.baseTimeLimit * data.bestKnownSolution));
    primalIntegral -= 1;
    primalIntegral *= 100;
    char primalIntegralString[32] = "";
    sprintf(primalIntegralString, "Primal Integral: %.10lf\n", primalIntegral);
    file.writeStringToFile(std::string(primalIntegralString));
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