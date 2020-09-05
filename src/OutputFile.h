#ifndef OUTPUTFILE_H
#define OUTPUTFILE_H

#include <cstdio>
#include <iostream>
#include <string>

class OutputFile {
    FILE* file;

  public:
    OutputFile() {}
    OutputFile(std::string name) : file(fopen(name.c_str(), "w")) {}
    // Write string str to OutputFile
    void writeStringToFile(std::string str);
    ~OutputFile() { pclose(file); }
};

#endif