#include "time.h"

int get_age(int year_born){
    time_t t = time(NULL); // From "time.h"
    struct tm tm = *localtime(&t);
    return tm.tm_year + 1900 - year_born;
}