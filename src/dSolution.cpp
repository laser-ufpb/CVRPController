#include "dSolution.h"

bool dSolution::checkSolution() {
    double solCost          = 0;
    vector< bool > isNodeIn = vector< bool >(instance->dimension, false);
    isNodeIn[0]             = true;

    for(vector< int > route : routes) {
        if(route[route.size() - 1] == 0)
            route.pop_back();

        solCost += instance->getEdgeWeight(0, route.at(0));

        int demanda = instance->demand[route.at(0)];
        if(demanda > instance->capacity)
            return false;

        isNodeIn[route.at(0)] = !(isNodeIn[route.at(0)] && true);

        for(int i = 1; i < route.size(); i++) {

            solCost += instance->getEdgeWeight(route.at(i - 1), route.at(i));

            demanda += instance->demand[route.at(i)];
            if(demanda > instance->capacity)
                return false;

            isNodeIn[route.at(i)] = !(isNodeIn[route.at(i)] && true);
        }
        solCost += instance->getEdgeWeight(route.at(route.size() - 1), 0);
    }

    for(bool i : isNodeIn)
        if(!i)
            return false;

    if(solCost != this->cost)
        return false;

    return true;
}

bool dSolution::parseLine(char* line) {
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

        routes.push_back(route);
    } else if(strstr(token, "Cost") != NULL) {
        token = strtok(NULL, " ");
        cost  = atof(token);
        flag  = true;
    }

    return flag;
}

string dSolution::getStats(std::chrono::high_resolution_clock::time_point beginTime, std::chrono::high_resolution_clock::time_point endTime, int passMark) {
    // std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(endTime - beginTime);

    char stats[256];
    sprintf(stats, "%.3lf %.3lf %.3lf\n", cost, ms.count() / 1000.0, (ms.count() / 1000.0) * ((double)passMark / CPU_BASE_REF));
    return string(stats);
}