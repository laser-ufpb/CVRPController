#include "Controlller.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    Controller controller(argc, argv);

    controller.run();

    return 0;
}
