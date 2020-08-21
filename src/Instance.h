#ifndef INSTANCE_H
#define INSTANCE_H

#include <iostream>
#include <string>
#include <vector>

class Instance {
  public:
    int dimension;
    int capacity;
    std::string name;
    std::vector< int > demand;
    std::vector< std::vector< int > > coord;

    Instance() {}
    Instance(const char* instanciaPath);
};

#endif