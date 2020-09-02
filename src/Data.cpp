#include "Data.h"

Data::Data(int argc, char* arguments[]) {
    this->competitorName = string(arguments[1]);
    this->path           = string(arguments[2]); // PATH TO EXECUTABLE
    this->isRounded      = atoi(arguments[3]);
    this->passMark       = atoi(arguments[4]);
    this->baseTimeLimit  = atof(arguments[5]);

    if(isRounded)
        this->bestKnownSolution = atoi(arguments[6]);
    else
        this->bestKnownSolution = atof(arguments[6]);

    this->baseSolution = bestKnownSolution * 1.1;
    this->isOptimal    = atoi(arguments[7]);
    this->execCommand  = string(arguments[8]);
    separateExecCommands();

    this->instance = Instance(this->path.c_str());
}

string Data::createHeader() {
    string header = "12th DIMACS Implementation Challenge: Vehicle Routing\nCVRP track\nCompetitor: " + competitorName + "\n";

    try {
        header += getOS();
        header += getCpuInfo();
    } catch(const char* e) {
        std::cout << e;
    }
    char aux[LEN];

    // Add machine's unique identifier
    sprintf(aux, "hostid: %d\n", gethostid());
    header += aux;

    sprintf(aux, "PassMark Single Thread Benchmark: %d\n"
                 "Time factor: %.2lf (baseline %d)\n"
                 "Instance: %s\n"
                 "EUC_2D distances rounded: %d\n"
                 "Standardized Time limit: %.0lf secs\n"
                 "Local Machine Time Limit: %.2lf secs\n",
            passMark,
            (passMark / (double)CPU_BASE_REF), CPU_BASE_REF,
            instance.name.c_str(),
            isRounded,
            baseTimeLimit,
            (baseTimeLimit / (passMark / (double)CPU_BASE_REF)));
    header += aux;

    if(isRounded)
        // Print Rounded BKS and Base Solution
        sprintf(aux, "Base solution: %.3lf\n"
                     "BKS: %.0lf\n"
                     "Optimal: %d\n",
                baseSolution, bestKnownSolution, isOptimal);
    else
        // Print Not Runded BKS and Base Solution
        sprintf(aux, "Base solution: %.3lf\n"
                     "BKS: %.3lf\n"
                     "Optimal: %d\n",
                baseSolution, bestKnownSolution, isOptimal);

    header += aux;
    header += getTime() + "\n";

    //Get timestamp since epoch
    std::time_t secondsSinceEpoch = std::time(0);
    sprintf(aux, "timestamp: %d\n", secondsSinceEpoch);
    header += aux;
    header += "Solution Cost - Local Time Machine - Standarized Time\n";
    return header;
}

// Get local time
string Data::getTime() {
    std::locale::global(std::locale("en_US.UTF-8"));
    std::time_t t;
    std::time(&t);
    std::tm* tm = std::localtime(&t);

    char string_time[LEN];

    if(!std::strftime(string_time, LEN - 1, "%c", tm))
        throw "Error: getTime() is NULL.";

    return string(string_time);
}

// Get OS data;
string Data::getOS() {
    char string_os[LEN];
    FILE* fp = popen("lsb_release -ds", "r");

    if(!fp)
        throw "ERROR: I can't open process lsb_realease. Maybe not running on Linux.";

    fgets(string_os, LEN - 1, fp);
    pclose(fp);

    return string(string_os);
}

// Get CPU model and specs
string Data::getCpuInfo() {
    FILE* cpuinfo = fopen("/proc/cpuinfo", "rb");

    if(!cpuinfo)
        throw("ERROR: I can't open /proc/cpuinfo. Maybe not running on Linux.");

    char* line_start = (char*)malloc(LEN * sizeof(char));
    size_t size      = 0;

    while(getdelim(&line_start, &size, '\n', cpuinfo) != -1) {
        if(!std::strncmp("model name", line_start, 10)) {
            break;
        }
    }

    fclose(cpuinfo);

    return string((line_start + 13));
}

// Get name of output file
string Data::getNameOfOutputFile() {
    return "DIMACS-CVRP-" + competitorName + "-" + instance.name + ".out";
}

// Get Argvs as a vector of cstrings
vector< char* > Data::getExecCommandArgvs() {
    vector< char* > tokens;

    for(int i = 0; i < argvs.size(); i++)
        tokens.push_back(&argvs[i][0]);

    tokens.push_back(NULL);
    return tokens;
}

// Separate exec command into tokens
void Data::separateExecCommands() {
    istringstream stringStream(this->execCommand);
    do {
        string argv;
        stringStream >> argv;
        argvs.push_back(argv);
    } while(stringStream);
}