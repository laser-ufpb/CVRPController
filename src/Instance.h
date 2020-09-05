#ifndef INSTANCE_H
#define INSTANCE_H

#include <iostream>
#include <math.h>
#include <string.h>
#include <string>
#include <vector>

class Instance {
  public:
    int dimension;
    int capacity;
    std::string name;
    bool isExplicit;
    std::vector< int > demand;
    std::vector< std::vector< double > > coordOrMatrix;

    Instance() {}
    Instance(const char* instanciaPath);
    // Get edge weight between two vertices "from" and "to"
    double getEdgeWeight(int from, int to);
};

#endif