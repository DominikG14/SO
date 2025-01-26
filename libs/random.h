#pragma once

#include <stdlib.h>
#include <unistd.h>


int rand_int(int min, int max){
    return min + rand() % (max - min + 1);
}

void imitate_time(int time){
    if(SLEEP_ACTIVE) sleep(time);
}