#ifndef INSTANCE_H
#define INSTANCE_H

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <string.h>

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
    double getEdgeWeight(int from, int to);
};

#endif