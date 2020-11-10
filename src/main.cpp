#include "Controller.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("ERROR: Wrong usage. Please checks DIMACS Competition Rules.\nExample:\n\t./Controller Wolverine X-n120-k6.vrp 1 2064 1800 13332 1 Solver1\n");
        exit(-1);
    }
    Controller controller(argc, argv);

    controller.run();

    return 0;
}
