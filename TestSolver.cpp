#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#define SECONDS 1e6
int main(int argc, char const* argv[]) {
    uint8_t a = 0;
    srand(time(NULL));
    
    while(a < 100) {
        printf("solution: %d\n", (uint8_t)a);
        fflush(stdout);
        a += 1;
        usleep(0.8 * SECONDS);
    }
    return 0;
}
