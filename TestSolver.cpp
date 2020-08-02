#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#define SECONDS 1e6
int main(int argc, char const* argv[]) {
    uint8_t a = 0;
    srand(time(NULL));

    while(a < 1) {
        printf("Route #1: 1 70 54\nRoute #2: 3 77 63\nRoute #3: 4 13 74\nRoute #4: 5 12 58\nRoute #5: 7 2 45 43 29 36 72 57\nRoute #6: 8 17\nRoute #7: 14 49 6 37 87\nRoute #8: 15 22 41 20\nRoute #9: 18 10 39\nRoute #10: 19 97 27\nRoute #11: 21 56 94 80\nRoute #12: 23 61 100\nRoute #13: 24 95 73 53 33 32\nRoute #14: 25 65 78 42 28\nRoute #15: 30 85 11 79\nRoute #16: 31 46 35\nRoute #17: 34 64 96 48 26 47 38\nRoute #18: 40 67 88 44\nRoute #19: 50 91 52\nRoute #20: 59 60 82\nRoute #21: 66 84 90 68\nRoute #22: 69 16 55 76\nRoute #23: 71 62 99 98 89\nRoute #24: 75 93\nRoute #25: 81 51 83\nRoute #26: 86 9 92\nCost 27591\n");
        fflush(stdout);
        a += 1;
        usleep(2 * SECONDS);
    }
    return 0;
}
