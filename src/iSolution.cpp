#include "iSolution.h"

bool iSolution::checkSolution() {
    int solCost             = 0;
    vector< bool > isNodeIn = vector< bool >(instance->dimension, false);
    isNodeIn[0]             = true;

    for(vector< int > route : routes) {
        if(route[route.size() - 1] == 0)
            route.pop_back();

        solCost += (int)round(sqrt(pow((instance->coord[0][1] - instance->coord[route.at(0)][1]), 2) + pow((instance->coord[0][2] - instance->coord[route.at(0)][2]), 2)));

        int demanda = instance->demand[route.at(0)];
        if(demanda > instance->capacity)
            return false;

        isNodeIn[route.at(0)] = !(isNodeIn[route.at(0)] && true);

        for(int i = 1; i < route.size(); i++) {

            solCost += (int)round(sqrt(pow((instance->coord[route.at(i-1)][1] - instance->coord[route.at(i)][1]), 2) + pow((instance->coord[route.at(i-1)][2] - instance->coord[route.at(i)][2]), 2)));

            demanda += instance->demand[route.at(i)];
            if(demanda > instance->capacity)
                return false;

            isNodeIn[route.at(i)] = !(isNodeIn[route.at(i)] && true);
        }
        solCost += (int)round(sqrt(pow((instance->coord[route.at(route.size()-1)][1] - instance->coord[0][1]), 2) + pow((instance->coord[route.at(route.size()-1)][2] - instance->coord[0][2]), 2)));
    }

    for(bool i : isNodeIn)
        if(!i)
            return false;

    if(solCost != this->cost)
        return false;

    return true;
}

bool iSolution::parseLine(char* line) {
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
        cost  = atoi(token);
        flag  = true;
    }

    return flag;
}

string iSolution::getStats(std::chrono::high_resolution_clock::time_point beginTime, int passMark){
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds ms                     = std::chrono::duration_cast< std::chrono::milliseconds >(t1 - beginTime);

    char stats[256];
    sprintf(stats, "%d %.3lf %.3lf\n", cost, (ms.count() / 1000.0) * ((double) passMark / CPU_BASE_REF), ms.count() / 1000.0);
    return string(stats);
}