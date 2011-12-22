#include "mylib.h"

void get_time() {

    time_t timep;
    struct tm *p;

    time(&timep);
    p = localtime(&timep);
    printf("%d-%d-%d ", (1900+p->tm_year), (1+p->tm_mon), p->tm_mday);
    printf("%02d:%02d:%02d", p->tm_hour, p->tm_min, p->tm_sec);
}
