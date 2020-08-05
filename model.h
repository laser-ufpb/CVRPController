#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

struct tData {
    string competitorName;
    string path;
    string execCommand;
    double baseTimeLimit;
    double baseSolution;
    double bestKnownSolution;
    int isOptimal;
    int isRounded;
    int passMark;

    tData(int argc, char* arguments[]) {
        competitorName    = string(arguments[1]);
        path              = string(arguments[2]); // PATH TO EXECUTABLE
        isRounded         = atoi(arguments[3]);
        passMark          = atoi(arguments[4]);
        baseTimeLimit     = atof(arguments[5]);
        bestKnownSolution = atof(arguments[6]);
        baseSolution      = bestKnownSolution * 1.1;
        isOptimal         = atoi(arguments[7]);
        execCommand       = string(arguments[8]);
    }
};

struct tSolution {
    vector< vector< int > > routes;
    double cost;
    double baseTime;
    double localTime;

    // tSolution() {}
    // tSolution(double cost, double baseTime, double localTime) : cost(cost), baseTime(baseTime), localTime(localTime) {}
};

struct tInstance {
    int dimension;
    string instanceName;
    vector< int > demand;
    vector< vector< double > > distanceMatrix;
    int capacity;

    tInstance(const char* instanciaPath, bool isRounded) {
        FILE* file = fopen(instanciaPath, "r");

        if(!file) {
            std::cout << "ERROR: Instance path wrong." << std::endl;
            exit(EXIT_FAILURE);
        }

        char* name = new char[64];
        char* aux  = new char[200];
        fscanf(file, "NAME : %s\n", name);
        instanceName = string(name);
        fscanf(file, "COMMENT : %99[^\n]\n", aux);
        fscanf(file, "TYPE : %99[^\n]\n", aux);
        fscanf(file, "DIMENSION : %d\n", &this->dimension);
        fscanf(file, "EDGE_WEIGHT_TYPE : %99[^\n]\n", aux);
        fscanf(file, "CAPACITY : %d\n", &capacity);
        demand = std::vector< int >(this->dimension, 0);
        int lixo;

        fscanf(file, "NODE_COORD_SECTION:\n");
        this->distanceMatrix                    = std::vector< std::vector< double > >(dimension, std::vector< double >(dimension, 0.0));
        std::vector< std::vector< int > > coord = std::vector< std::vector< int > >(dimension, std::vector< int >(3, 0));

        for(int i = 0; i < dimension; i++) {
            for(int j = 0; j < 3; j++) {
                fscanf(file, "%d", &coord[i][j]);
            }
        }

        fscanf(file, "\n");
        fscanf(file, "DEMAND_SECTION:\n");
        for(int i = 0; i < dimension; i++) {
            fscanf(file, "%d %d", &lixo, &this->demand[i]);
        }

        for(int i = 0; i < dimension; i++) {
            for(int j = 0; j < dimension; j++) {

                this->distanceMatrix[i][j] = (isRounded) ? (int)round(sqrt(pow((coord[i][1] - coord[j][1]), 2) + pow((coord[i][2] - coord[j][2]), 2))) : sqrt(pow((coord[i][1] - coord[j][1]), 2) + pow((coord[i][2] - coord[j][2]), 2));
            }
        }

        delete aux;
        delete name;
        fclose(file);
    }

    void printData() {
        printf("DIMENSION: %d\n", dimension);

        printf("CAPACITY: %d\n", capacity);

        for(int i = 0; i < dimension; i++) {
            printf("DEMAND: %d %d\n", i, this->demand[i]);
        }
        for(int i = 0; i < dimension; i++) {
            for(int j = 0; j < dimension; j++) {
                printf("%lf ", distanceMatrix[i][j]);
            }
            puts("");
        }
    }

    bool checkInstance(tSolution sol) {
        double solCost       = 0;
        vector<bool> isNodeIn = vector<bool>(dimension, false);
        isNodeIn[0] = true;

        for(vector< int > route : sol.routes) {
            solCost += distanceMatrix[0][route.at(0)];

            int demanda = demand[route.at(0)];
            if(demanda > capacity)
                return false;

            isNodeIn[route.at(0)] = !(isNodeIn[route.at(0)] && true);
            
            for(int i = 1; i < route.size(); i++) {
                solCost += distanceMatrix[route.at(i - 1)][route.at(i)];

                demanda += demand[route.at(i)];
                if(demanda > capacity)
                    return false;

                isNodeIn[route.at(i)] = !(isNodeIn[route.at(i)] && true);
            }
            solCost += distanceMatrix[route.at(route.size() - 1)][0];
        }

        for (bool i : isNodeIn)
            if(!i)
                return false;
        
        if(solCost != sol.cost)
            return false;

        return true;
    }
};

#endif