#include "Data.h"

Data::Data(int argc, char* arguments[]) {
    this->competitorName = string(arguments[1]);
    this->path           = string(arguments[2]); // PATH TO EXECUTABLE
    this->isRounded      = atoi(arguments[3]);
    this->passMark       = atoi(arguments[4]);
    this->baseTimeLimit  = atof(arguments[5]);

    if(isRounded) {
        this->euc_2d            = "1";
        this->bestKnownSolution = atoi(arguments[6]);
    } else {
        this->euc_2d            = "0";
        this->bestKnownSolution = atof(arguments[6]);
    }
    this->baseSolution = bestKnownSolution * 1.1;
    this->isOptimal    = atoi(arguments[7]);
    this->solverName   = string(arguments[8]);

    this->instance = Instance(this->path.c_str());
}

string Data::createHeader() {
    string header = "12th DIMACS Implementation Challenge: Vehicle Routing\nCVRP track\nCompetitor: " + competitorName + "\n";

    try {
        header += getOS();
        header += getCpuStats();
    } catch(const char* e) {
        std::cout << e;
    }
    char aux[LEN];

// Add machine's unique identifier
#ifdef __linux__
    sprintf(aux, "hostid: %ld\n", gethostid());
#elif __OpenBSD__ || __freebsd__
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
    pclose(fp);
#elif __APPLE__ && __MACH__
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
    pclose(fp);
#endif

    header += aux;

    sprintf(aux, "PassMark Single Thread Benchmark: %d\n"
                 "Time factor: %.2lf (baseline %d)\n"
                 "Instance: %s\n"
                 "EUC_2D distances rounded: %d\n"
                 "Standardized Time limit: %.0lf secs\n"
                 "Local Machine Time Limit: %.0lf secs\n",
            passMark,
            (passMark / (double)CPU_BASE_REF), CPU_BASE_REF,
            instance.name.c_str(),
            isRounded,
            baseTimeLimit,
            round(baseTimeLimit / (passMark / (double)CPU_BASE_REF)));
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
    vector< char* > tokens = {&(this->solverName[0]), &(this->path[0]), &(this->euc_2d[0]), NULL};

    return tokens;
}
