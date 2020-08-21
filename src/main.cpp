#include "Controlller.h"
#include <iostream>
using namespace std;

// double getPrimalIntegral(Data data, vector< tSolution > solutions) {
//     int n                 = solutions.size() - 1;
//     double primalIntegral = 0;
//     for(int i = 1; i < solutions.size(); i++) {
//         // v(i-1)*(t(i)-t(i-1))
//         primalIntegral += (solutions[i - 1].cost * (solutions[i].baseTime - solutions[i - 1].baseTime));
//     }
//     primalIntegral += (solutions[n].cost * (data.baseTimeLimit - solutions[n].baseTime));
//     primalIntegral /= (data.baseTimeLimit * data.bestKnownSolution);
//     primalIntegral -= 1;
//     primalIntegral *= 100;
//     return primalIntegral;
// }

//  Solutions is set v and t, so v(0) = solutions[0].cost and t[0] = solutions[0].localTime;
// void writeOutput(Data& data, vector< tSolution > solutions) {

//     // DATE
//     std::time(&t);
//     tm = std::localtime(&t);
//     if(!std::strftime(string_time, 64, "%c", tm)) {
//         std::cout << "ERROR" << std::endl;
//         _exit(-1);
//     };
//     fprintf(output, "%s\n", string_time);

//     fprintf(output, "Primal Integral: %.10lf\n", getPrimalIntegral(data, solutions));
//     fclose(output);
//     free(string_cpu - 13);
// }

int main(int argc, char* argv[]) {
    Controller controller(argc, argv);

    controller.run();

    return 0;
}
