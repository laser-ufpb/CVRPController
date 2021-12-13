#include "Data.h"

Data::Data(int argc, char* arguments[]) {
    this->competitorName = string(arguments[1]);
    this->path           = string(arguments[2]); // PATH TO EXECUTABLE
    this->distanceType   = atoi(arguments[3]);
    this->passMark       = atoi(arguments[4]);
    this->baseTimeLimit  = atof(arguments[5]);
    this->timeLimitStr = std::to_string((int) round(baseTimeLimit / ((double) passMark / CPU_BASE_REF)));

    if(distanceType == 0) {
        this->euc_2d            = "0";
        this->bestKnownSolution = atof(arguments[6]);
    } else if(distanceType == 1) {
        this->euc_2d            = "1";
        this->bestKnownSolution = atoi(arguments[6]);
    } else if(distanceType == 2) {
        this->euc_2d            = "2";
        this->bestKnownSolution = atoi(arguments[6]);
    } else {
        std::cout << "ERROR: Distance type must be 0 (EUC2D not rounded), 1 (EUC2D rounded), or 2 (Explicit)." << std::endl;
        exit(EXIT_FAILURE);
    }

    this->baseSolution = bestKnownSolution * 1.1;
    this->isOptimal    = atoi(arguments[7]);
    this->solverName   = string(arguments[8]);

    this->instance = Instance(this->path.c_str());
}

string Data::createHeader() {
    string header = "12th DIMACS Implementation Challenge: Vehicle Routing\nCVRP track\nController version: December 13, 2021\nCompetitor: " 
                    + competitorName + "\n";

    try {
        header += getOS();
        header += getCpuStats();
    } catch(const char* e) {
        std::cout << e;
    }
    char aux[LEN] = {};

// Add machine's unique identifier
#ifdef __linux__
    hash<string> hasher;
    FILE* file = fopen("/etc/machine-id", "r");
    if(!file) {
        std::cout << "ERROR: This machine does not have /etc/machine-id, then the hostid cannot be generated!" << std::endl;
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%s", aux);
    string machineid = string(aux);
    fclose(file);
    size_t hostid = hasher(machineid);
    sprintf(aux, "hostid: %zu\n", hostid);
#elif __OpenBSD__ || __freebsd__
    header += "hostid: ";
    char string_hostid[LEN];
    FILE* fp = popen("ifconfig | awk '/lladdr/ {print $2}'", "r");
    if(!fp) throw "ERROR: I can't open process ifconfig.";
    fgets(string_hostid, LEN - 1, fp);
    char* _aux = string_hostid;
    int added  = 0;
    while(isalnum(*_aux) || *_aux == ':') {
        if(*_aux != ':') {
            *(aux + added) = *_aux;
            added++;
        }
        _aux++;
    }
    *(aux + added) = '\n';
    added++;
    pclose(fp);
#elif __APPLE__ && __MACH__
    header += "hostid: ";
    char string_hostid[LEN];
    FILE* fp = popen("ifconfig | awk '/ether/ {print $2}'", "r");
    if(!fp) throw "ERROR: I can't open process ifconfig.";
    fgets(string_hostid, LEN - 1, fp);
    char* _aux = string_hostid;
    int added  = 0;
    while(isalnum(*_aux) || *_aux == ':') {
        if(*_aux != ':') {
            *(aux + added) = *_aux;
            added++;
        }
        _aux++;
    }
    *(aux + added) = '\n';
    added++;
    pclose(fp);
#endif

    header += aux;

    sprintf(aux, "PassMark Single Thread Benchmark: %d\n"
                 "Time factor: %.2lf (baseline %d)\n"
                 "Instance: %s\n"
                 "Distance type: %d\n"
                 "Standardized Time limit: %.0lf secs\n"
                 "Local Machine Time Limit: %.0lf secs\n",
            passMark,
            (passMark / (double)CPU_BASE_REF), CPU_BASE_REF,
            instance.name.c_str(),
            distanceType,
            baseTimeLimit,
            round(baseTimeLimit / (passMark / (double)CPU_BASE_REF)));
    header += aux;

    if(distanceType != 0)
        // Print Rounded BKS and Base Solution
        sprintf(aux, "Base solution: %.3lf\n"
                     "BKS: %.0lf\n"
                     "Optimal: %d\n",
                baseSolution, bestKnownSolution, isOptimal);
    else
        // Print Not Runded BKS and Base Solution
        sprintf(aux, "Base solution: %.3lf\n"
                     "BKS: %.2lf\n"
                     "Optimal: %d\n",
                baseSolution, bestKnownSolution, isOptimal);

    header += aux;
    header += getTime() + "\n";

    //Get timestamp since epoch
    std::time_t secondsSinceEpoch = std::time(0);
    sprintf(aux, "timestamp: %li\n", secondsSinceEpoch);
    header += aux;
    header += "Solution value, local machine time, standardized time\n";
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
#ifdef __linux__
    FILE* fp = popen("lsb_release -ds", "r");
    if(!fp) throw "ERROR: I can't open process lsb_realease. Maybe not running on Linux.";
#elif __OpenBSD__ || __freebsd__
    FILE* fp = popen("uname -rs", "r");
    if(!fp) throw "ERROR: I can't open process uname.";
#elif __APPLE__ && __MACH__
    FILE* fp = popen("sw_vers -productName", "r");
    if(!fp) throw "ERROR: I can't open process sw_vers.";
#endif

    fgets(string_os, LEN - 1, fp);
    pclose(fp);

    return string(string_os);
}

// Get CPU model and specs
string Data::getCpuStats() {
    char string_os[LEN];
#ifdef __linux__
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
#elif __OpenBSD__ || __freebsd__
    FILE* fp = popen("sysctl -n hw.model", "r");
    if(!fp) throw "ERROR: I can't open process sysctl to get CPU info.";

    fgets(string_os, LEN - 1, fp);
    pclose(fp);

    return string(string_os);
#elif __APPLE__ && __MACH__
    FILE* fp = popen("sysctl -n machdep.cpu.brand_string", "r");
    if(!fp) throw "ERROR: I can't open process sysctl to get CPU info.";

    fgets(string_os, LEN - 1, fp);
    pclose(fp);

    return string(string_os);
#endif
    throw "Error OS not supported.";
}

// Get name of output file
string Data::getNameOfOutputFile() {
    return "DIMACS-CVRP-" + competitorName + "-" + instance.name + ".out";
}

// Get Argvs as a vector of cstrings
vector< char* > Data::getExecParameters() {
    vector< char* > tokens = {&(this->solverName[0]), &(this->path[0]), &(this->euc_2d[0]), &(this->timeLimitStr[0]), NULL};

    return tokens;
}
