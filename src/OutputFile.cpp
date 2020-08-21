#include "OutputFile.h"

void OutputFile::writeStringToFile(std::string str) {
    if(fprintf(this->file, "%s", str.c_str()) <= 0) throw "Error: Write unsucessfull";
    fflush(this->file);
}