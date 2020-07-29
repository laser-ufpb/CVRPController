#include <chrono>
#include <cstdio>
#include <cstring>
#include <ctime>

#define PATH_MAX 32
int main() {
    FILE* fp;
    int status;
    char path[PATH_MAX];
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::milliseconds milliseconds;
    fp = popen("./test", "r");
    if(fp == NULL)
        /* Handle error */;
    Clock::time_point t0 = Clock::now();
    while(fgets(path, PATH_MAX, fp) != NULL) {
        Clock::time_point t1 = Clock::now();
        milliseconds ms      = std::chrono::duration_cast< milliseconds >(t1 - t0);
        printf("%s | in %f ms\n", path, (float)ms.count() / 1000);
        fflush(stdout);
    }

    status = pclose(fp);

    return 0;
}
