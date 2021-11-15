#include "Instance.h"

Instance::Instance(const char* instanciaPath) {
    FILE* file = fopen(instanciaPath, "r");

    if(!file) {
        std::cout << "ERROR: Instance path wrong." << std::endl;
        exit(EXIT_FAILURE);
    }

    char name[64];
    char edgeWeightType[64];
    char aux[200];

    fscanf(file, "NAME : %s\n", name);
    this->name = std::string(name);
    fscanf(file, "COMMENT : %*[^\n]\n");
    fscanf(file, "TYPE : %*[^\n]\n");
    fscanf(file, "DIMENSION : %d\n", &this->dimension);
    fscanf(file, "EDGE_WEIGHT_TYPE : %s\n", edgeWeightType);
    
    if(!strcmp(edgeWeightType, "EUC_2D"))
        this->isExplicit = false;
    else
        this->isExplicit = true;

    if(isExplicit) {
        fscanf(file, "EDGE_WEIGHT_FORMAT : %s\n", aux);
        fscanf(file, "NODE_COORD_TYPE : %*[^\n]\n");
    }

    fscanf(file, "CAPACITY : %d\n", &capacity);

    if(!isExplicit){
        fscanf(file, "NODE_COORD_SECTION\n");
        this->coordOrMatrix = std::vector< std::vector< double > >(dimension, std::vector< double >(3, 0));

        for(int i = 0; i < dimension; i++) {
            for(int j = 0; j < 3; j++) {
                fscanf(file, "%lf", &coordOrMatrix[i][j]);
            }
        }
    }else{
        fscanf(file, "EDGE_WEIGHT_SECTION\n");
        this->coordOrMatrix = std::vector< std::vector< double > >(dimension, std::vector< double >(dimension,0));

        for(int i = 0; i < dimension; i++) {
            for(int j = 0; j < dimension; j++) {
                if(i == j)
                    break;
                    
                fscanf(file, "%lf", &coordOrMatrix[i][j]);
                coordOrMatrix[j][i] = coordOrMatrix[i][j];
            }
        }

        fscanf(file, "\n");
        fscanf(file, "NODE_COORD_SECTION\n");
        int vertex_index;
        double x, y;
        for (int i = 0; i < dimension; ++i) {
            fscanf(file, "%d %lf %lf", &vertex_index, &x, &y);
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

double Instance::getEdgeWeight(int from, int to){
    if(!isExplicit)
        return sqrt(pow((coordOrMatrix[from][1] - coordOrMatrix[to][1]), 2) + pow((coordOrMatrix[from][2] - coordOrMatrix[to][2]), 2));
    else 
        return coordOrMatrix[from][to];
}