#include "Instance.h"

Instance::Instance(const char* instanciaPath) {
    FILE* file = fopen(instanciaPath, "r");

    if(!file) {
        std::cout << "ERROR: Instance path wrong." << std::endl;
        exit(EXIT_FAILURE);
    }

    char name[64];
    char aux[200];

    fscanf(file, "NAME : %s\n", name);
    this->name = std::string(name);
    fscanf(file, "COMMENT : %99[^\n]\n", aux);
    fscanf(file, "TYPE : %99[^\n]\n", aux);
    fscanf(file, "DIMENSION : %d\n", &this->dimension);
    fscanf(file, "EDGE_WEIGHT_TYPE : %99[^\n]\n", aux);
    fscanf(file, "CAPACITY : %d\n", &capacity);

    fscanf(file, "NODE_COORD_SECTION:\n");
    this->coord = std::vector< std::vector< int > >(dimension, std::vector< int >(3, 0));

    for(int i = 0; i < dimension; i++) {
        for(int j = 0; j < 3; j++) {
            fscanf(file, "%d", &coord[i][j]);
        }
    }

    fscanf(file, "\n");
    fscanf(file, "DEMAND_SECTION:\n");

    int lixo;
    this->demand = std::vector< int >(this->dimension, 0);
    for(int i = 0; i < dimension; i++) {
        fscanf(file, "%d %d", &lixo, &this->demand[i]);
    }

    fclose(file);
}