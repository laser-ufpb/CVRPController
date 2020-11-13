#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#define SECONDS 1e6
int main(int argc, char const* argv[]) {
    uint8_t a = 0;
    srand(time(NULL));

    // while(a < 2) {
        printf("Route #1: 14 57 82 37 43 64\nRoute #2: 18 55 26 24\nRoute #3: 38 27 52\nRoute #4: 75 85 51 62\nRoute #5: 66 74 69 20\nRoute #6: 73 33 93 34 71 30\nRoute #7: 100 21 56 8\nRoute #8: 59 25 10\nRoute #9: 49 2 7 13 90\nRoute #10: 3 87 60 77\nRoute #11: 80 17 94\nRoute #12: 11 61 99 81\nRoute #13: 68 84 16 45\nRoute #14: 97 23 44 35\nRoute #15: 63 36 29 96\nRoute #16: 5 50 47 48\nRoute #17: 1 86 31\nRoute #18: 83 79 95\nRoute #19: 4 76 9 92\nRoute #20: 54 70 46\nRoute #21: 89 98 91 19\nRoute #22: 65 42 78 6 28\nRoute #23: 22 12 58\nRoute #24: 40 88 67 72 39\nRoute #25: 15 41 53 32\nCost 37031\n");

        fflush(stdout);
        a += 1;
        usleep(2 * SECONDS);

        printf("Route #1: 32 15 92 16\nRoute #2: 61 98 23\nRoute #3: 12 35 41\nRoute #4: 80 17 22\nRoute #5: 18 68 86\nRoute #6: 48 11 85 75\nRoute #7: 88 36 43 3\nRoute #8: 74 55 76 90\nRoute #9: 82 91 97\nRoute #10: 5 1 31 33\nRoute #11: 44 67 60\nRoute #12: 58 40 64 34 8\nRoute #13: 9 69 13 4\nRoute #14: 66 78 72 57 28 63\nRoute #15: 56 94 96 59\nRoute #16: 52 62 51\nRoute #17: 81 71 99 83\nRoute #18: 79 30 20\nRoute #19: 70 54 84\nRoute #20: 42 6 37 29 87\nRoute #21: 26 45 2 49 7 65 77\nRoute #22: 14 10 25 39\nRoute #23: 53 73 95 24 46\nRoute #24: 21 100 27 89 47 38\nRoute #25: 93 19 50\nCost 33058\n");
        fflush(stdout);
        usleep(2 * SECONDS);
        
        printf("Route #1: 1 70 54\nRoute #2: 3 77 63\nRoute #3: 4 13 74\nRoute #4: 5 12 58\nRoute #5: 7 2 45 43 29 36 72 57\nRoute #6: 8 17\nRoute #7: 14 49 6 37 87\nRoute #8: 15 22 41 20\nRoute #9: 18 10 39\nRoute #10: 19 97 27\nRoute #11: 21 56 94 80\nRoute #12: 23 61 100\nRoute #13: 24 95 73 53 33 32\nRoute #14: 25 65 78 42 28\nRoute #15: 30 85 11 79\nRoute #16: 31 46 35\nRoute #17: 34 64 96 48 26 47 38\nRoute #18: 40 67 88 44\nRoute #19: 50 91 52\nRoute #20: 59 60 82\nRoute #21: 66 84 90 68\nRoute #22: 69 16 55 76\nRoute #23: 71 62 99 98 89\nRoute #24: 75 93\nRoute #25: 81 51 83\nRoute #26: 86 9 92\nCost 27591\n");
        fflush(stdout);
        usleep(2 * SECONDS);

    // }
    return 0;
}
